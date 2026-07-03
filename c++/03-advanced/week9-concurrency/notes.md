# Week 9 知识点笔记：并发编程

## 一、线程创建 — `std::thread`

### 1.1 基本用法

```cpp
#include <thread>
#include <iostream>

void hello(const std::string& name) {
    std::cout << "Hello, " << name << " from thread "
              << std::this_thread::get_id() << '\n';
}

int main() {
    std::thread t1(hello, "Alice");          // 启动线程
    std::thread t2([] { hello("Bob"); });    // lambda

    t1.join();   // 等待线程结束
    t2.join();

    return 0;
}
```

### 1.2 join vs detach

```cpp
void work() { /* ... */ }

std::thread t(work);

// join: 等待线程完成。调用后线程对象不再代表任何线程
t.join();

// detach: 分离线程，线程独立运行。调用后线程对象不再管理任何线程
t.detach();  // 危险：主线程退出后分离线程访问已销毁对象 = UB

// joinable: 检查线程对象是否代表一个活跃线程
if (t.joinable()) {
    t.join();  // 安全：确保在析构前 join 或 detach
}
```

**关键规则：`std::thread` 析构前必须调用 `join()` 或 `detach()`，否则 `std::terminate`。**

### 1.3 传递参数

```cpp
void update(int& value) { value *= 2; }

int x = 10;
std::thread t(update, std::ref(x));  // 必须用 std::ref 传引用
t.join();
// x == 20

// 线程函数参数默认按值拷贝到线程内部存储
// 指针传递需确保所指向对象在线程存活期间有效
```

### 1.4 获取线程信息

```cpp
std::thread t([] {});
std::cout << "Thread ID: " << t.get_id() << '\n';
std::cout << "Hardware concurrency: "
          << std::thread::hardware_concurrency() << '\n';

// std::this_thread 命名空间
std::this_thread::sleep_for(std::chrono::milliseconds(100));
std::this_thread::yield();  // 提示调度器让出时间片
```

## 二、互斥锁 — `std::mutex`

### 2.1 基本锁类型

```cpp
#include <mutex>

std::mutex mtx;                    // 不可重入，最常用
std::recursive_mutex rec_mtx;      // 可重入（同一线程可多次 lock）
std::timed_mutex timed_mtx;        // 支持 try_lock_for / try_lock_until
std::shared_mutex shared_mtx;      // C++17 读写锁
```

### 2.2 lock_guard（最简单）

```cpp
std::mutex mtx;
int counter = 0;

void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // 构造时 lock，析构时 unlock
    ++counter;
    // 即使抛出异常，lock_guard 析构也保证解锁
}
```

### 2.3 unique_lock（更灵活）

```cpp
std::mutex mtx;

void flexible_lock() {
    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);  // 不立即锁

    // 可以稍后手动上锁
    lock.lock();
    // ... critical section ...
    lock.unlock();      // 可提前解锁
    // ... non-critical work ...
    lock.lock();        // 重新上锁

    // 条件变量必须用 unique_lock
    // 析构时自动解锁
}
```

### 2.4 scoped_lock（C++17，避免死锁）

```cpp
std::mutex mtx1, mtx2;

void transfer() {
    // 同时锁定两个 mutex，避免死锁（内部用 std::lock）
    std::scoped_lock lock(mtx1, mtx2);
    // ... 同时操作两个共享资源 ...
}

// 等价于
void transfer_manual() {
    std::lock(mtx1, mtx2);                     // 安全地同时获取两把锁
    std::lock_guard<std::mutex> lock1(mtx1, std::adopt_lock);
    std::lock_guard<std::mutex> lock2(mtx2, std::adopt_lock);
}
```

### 2.5 死锁演示与预防

```cpp
// 死锁示例
std::mutex a, b;
std::thread t1([&] {
    std::lock_guard lk1(a);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::lock_guard lk2(b);  // 等待 b — 但 b 被 t2 持有
});
std::thread t2([&] {
    std::lock_guard lk2(b);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    std::lock_guard lk1(a);  // 等待 a — 死锁！
});

// 解决方案: scoped_lock / std::lock
std::thread t3([&] {
    std::scoped_lock lock(a, b);  // 同时获取，避免死锁
});
```

## 三、条件变量 — `std::condition_variable`

### 3.1 基本模式

```cpp
#include <mutex>
#include <condition_variable>
#include <queue>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> data;
bool done = false;

// 生产者
void producer() {
    for (int i = 0; i < 10; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        {
            std::lock_guard<std::mutex> lock(mtx);
            data.push(i);
        }
        cv.notify_one();  // 唤醒一个消费者
    }
    {
        std::lock_guard<std::mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();
}

// 消费者
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return !data.empty() || done; });
        // wait 等价于: while (!pred()) { cv.wait(lock); }

        while (!data.empty()) {
            int val = data.front();
            data.pop();
            lock.unlock();     // 处理数据时不持锁
            std::cout << val << ' ';
            lock.lock();
        }

        if (done) break;
    }
}
```

### 3.2 wait 的两种形式

```cpp
std::unique_lock<std::mutex> lock(mtx);

// 形式1: 无 predicate — 必须手动处理虚假唤醒
cv.wait(lock);
// 等价于: while (!condition) cv.wait(lock);  ← 自己写

// 形式2: 有 predicate — 推荐使用，自动处理虚假唤醒
cv.wait(lock, [] { return ready; });
// 等价于: while (!ready) cv.wait(lock);
```

### 3.3 notify 时机

```cpp
void worker() {
    // 1. 修改共享数据时持锁
    {
        std::lock_guard<std::mutex> lock(mtx);
        ready = true;
    }
    // 2. 通知可以放到锁外（推荐：减少唤醒后立即阻塞的竞态）
    cv.notify_all();

    // 也可在锁内通知（唤醒线程立即发现仍锁着，再阻塞）
    // 通常锁外通知性能略好
}
```

## 四、原子操作 — `std::atomic`

### 4.1 基本原子类型

```cpp
#include <atomic>

std::atomic<int> counter{0};
std::atomic<bool> flag{false};
std::atomic<int*> ptr{nullptr};

// 基本操作
counter.store(42);                 // 原子写入
int val = counter.load();          // 原子读取
int old = counter.exchange(99);    // 原子交换（返回旧值）

// compare_exchange_strong / weak — CAS 操作
int expected = 42;
bool success = counter.compare_exchange_strong(expected, 100);
// 如果 counter == 42，设为 100，返回 true
// 否则 expected 被更新为当前值，返回 false

// 整数原子类型特有操作
counter.fetch_add(1);             // 原子加，返回旧值
counter.fetch_sub(1);             // 原子减
counter++;                         // 等价于 fetch_add(1)
++counter;                         // 等价于 fetch_add(1) + 1
counter += 5;                      // 等价于 fetch_add(5) + 5

counter.fetch_and(0xFF);
counter.fetch_or(0x01);
counter.fetch_xor(0x01);

// is_lock_free: 检查是否真正无锁（平台相关）
std::cout << "counter is lock-free: " << counter.is_lock_free() << '\n';
```

### 4.2 内存序入门

```cpp
#include <atomic>

// 六种内存序，从强到弱：
// memory_order_seq_cst  — 顺序一致性（默认，最强，最慢）
// memory_order_acquire   — 获取（读）
// memory_order_release   — 释放（写）
// memory_order_acq_rel   — 获取-释放
// memory_order_consume   — 消费（不推荐）
// memory_order_relaxed   — 松弛（最弱，最快，仅保证原子性）

std::atomic<int> x{0}, y{0};
int r1, r2;

// 最简方案: 默认 seq_cst
x.store(1);  // memory_order_seq_cst

// 优化: 使用更弱的内存序
std::atomic<bool> ready{false};
int data = 0;  // 非原子

// 写线程
void writer() {
    data = 42;
    ready.store(true, std::memory_order_release);  // 确保 data 写入对 reader 可见
}

// 读线程
void reader() {
    while (!ready.load(std::memory_order_acquire)) { }  // 同步点
    assert(data == 42);  // 保证可见
}

// memory_order_relaxed: 仅保证原子性，无同步
std::atomic<int> usageCount{0};
void incrementUsage() {
    usageCount.fetch_add(1, std::memory_order_relaxed);  // 仅计数，不需同步
}
```

**内存序选择指南：**
- 默认 `seq_cst`：最容易推理，性能通常够用
- `acquire`/`release` 对：标志位同步（如自旋锁、发布-订阅）
- `relaxed`：纯计数器，不需要与其他内存操作同步时

## 五、Future/Promise/Async

### 5.1 std::async

```cpp
#include <future>
#include <iostream>

int compute(int x) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x * x;
}

int main() {
    // launch::async — 必须在新线程中执行
    // launch::deferred — 延迟求值（在 wait/get 时才执行，同线程）
    // launch::async | launch::deferred — 由实现决定（默认）
    std::future<int> fut = std::async(std::launch::async, compute, 5);

    std::cout << "Doing other work...\n";

    int result = fut.get();  // 阻塞直到结果就绪（只能调用一次！）
    std::cout << "Result: " << result << '\n';

    return 0;
}
```

### 5.2 std::future 和 std::shared_future

```cpp
std::future<int> fut = std::async([] { return 42; });

// get() 只能调用一次
int val = fut.get();
// int val2 = fut.get();  // 错误！future 已被移动

// 需要使用多次时 → shared_future
std::shared_future<int> shared = fut.share();  // 转移所有权
// 或: auto shared = std::async([] { return 42; }).share();

int a = shared.get();  // ok
int b = shared.get();  // ok，返回相同结果（拷贝）

// 状态查询
fut.valid();           // future 是否持有共享状态
fut.wait();            // 阻塞等待结果就绪
auto status = fut.wait_for(std::chrono::seconds(1));  // 等待一段时间
// 返回: future_status::ready / ::timeout / ::deferred
```

### 5.3 std::promise

```cpp
#include <future>

void producer(std::promise<int> prom) {
    try {
        int result = 42;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        prom.set_value(result);      // 设置结果
    } catch (...) {
        prom.set_exception(std::current_exception());  // 设置异常
    }
}

int main() {
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::thread t(producer, std::move(prom));  // promise 不可拷贝，必须移动

    std::cout << "Waiting...\n";
    try {
        int val = fut.get();  // 阻塞直到 promise 被设置
        std::cout << "Got: " << val << '\n';
    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << '\n';
    }

    t.join();
    return 0;
}
```

### 5.4 std::packaged_task

```cpp
#include <future>

int add(int a, int b) { return a + b; }

int main() {
    std::packaged_task<int(int, int)> task(add);  // 包装可调用对象
    std::future<int> fut = task.get_future();

    std::thread t(std::move(task), 3, 4);  // task 需移动

    std::cout << "3 + 4 = " << fut.get() << '\n';
    t.join();

    return 0;
}
```

## 六、call_once 与线程安全初始化

```cpp
#include <mutex>

// Meyer's Singleton — C++11 起线程安全
class Singleton {
public:
    static Singleton& getInstance() {
        static Singleton instance;  // C++11 保证线程安全的懒初始化
        return instance;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

private:
    Singleton() = default;
};

// std::call_once — 显式控制一次初始化
std::once_flag initFlag;
std::shared_ptr<SomeResource> resource;

void ensureInitialized() {
    std::call_once(initFlag, [] {
        resource = std::make_shared<SomeResource>(/* ... */);
    });
}
```

## 七、thread_local 存储期

```cpp
thread_local int threadId = 0;

void worker(int id) {
    threadId = id;
    std::cout << "Thread " << threadId << ": "
              << &threadId << '\n';  // 每个线程有独立的 threadId
}
```

## 八、常见陷阱

### 8.1 向线程传临时对象的引用

```cpp
void process(std::string& s) { s += "!"; }

// 错误：临时 string 按值拷贝到线程内部存储，然后传给 process 的是线程内部的临时对象
// std::thread t(process, "hello");  // 编译错误：临时对象不能绑定到非 const 引用

// 正确：显式传引用
std::string msg = "hello";
std::thread t(process, std::ref(msg));
t.join();
```

### 8.2 对 bool 标志使用 volatile 而非 atomic

```cpp
volatile bool running = true;  // 错误！不保证多线程可见性
std::atomic<bool> running = true;  // 正确
```

### 8.3 忘记 join/detach

```cpp
{
    std::thread t(work);
    // 离开作用域 → t.~thread() 被调 → std::terminate!
}

// 正确：使用 jthread（C++20）自动 join
{
    std::jthread t(work);  // 析构时自动 join
}
```

### 8.4 条件变量丢失通知

```cpp
// 错误：先通知，后等待
cv.notify_one();
cv.wait(lock);  // 永远等不到（通知已丢失）

// 正确：用 predicate 版本，依赖共享状态
cv.wait(lock, [] { return ready; });
```

### 8.5 数据竞争（data race）

```cpp
int counter = 0;

void increment() { ++counter; }  // data race — 未定义行为

// C++ 内存模型：两个线程同时访问同一内存位置，
// 至少一个是写操作，且都不是原子操作 → data race → UB
```

## 九、最佳实践

1. **默认用 `lock_guard`，需要条件变量时才用 `unique_lock`**
2. **`condition_variable::wait` 始终用 predicate 版本**
3. **多线程共享 bool 标志必须用 `atomic<bool>`，不是 `volatile`**
4. **用 `scoped_lock` 锁多个 mutex 避免死锁**
5. **最小化临界区长度：锁外做不需要同步的工作**
6. **`thread` 对象析构前确保已 join 或 detach**
7. **优先用 `std::async` + `future` 而非手动创建线程**
8. **Meyer's Singleton 是线程安全懒初始化的最简方式**
9. **`atomic` 默认内存序够用，除非性能分析显示瓶颈**
10. **用 `ThreadSanitizer` (`-fsanitize=thread`) 检测数据竞争**
