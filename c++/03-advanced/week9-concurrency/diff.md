# Week 9：并发编程 — Java vs C++ 差异对照

## 一、synchronized → mutex + lock_guard

```java
// Java — 语言内置同步关键字
public class Counter {
    private int count = 0;

    public synchronized void increment() {  // 方法级锁
        count++;
    }

    public void decrement() {
        synchronized (this) {               // 块级锁
            count--;
        }
    }
}
```

```cpp
// C++ — 需要显式锁对象，RAII 管理
#include <mutex>

class Counter {
    int count = 0;
    mutable std::mutex mtx;

public:
    void increment() {
        std::lock_guard<std::mutex> lock(mtx);  // RAII 自动解锁
        ++count;
    }

    void decrement() {
        std::scoped_lock lock(mtx);              // C++17，可锁多个
        --count;
    }
};
```

| 特性 | Java `synchronized` | C++ `mutex` |
|------|-------------------|-------------|
| 语言级别 | 关键字，JVM 内置 | 标准库类，无语法糖 |
| 自动释放 | 退出同步块自动释放 | `lock_guard` RAII 析构释放 |
| 可重入 | 是（同一线程可重入同一个锁） | `std::recursive_mutex` 才可重入；`std::mutex` 不可重入 |
| 等待/通知 | `wait()` / `notify()` / `notifyAll()` | `condition_variable` + `wait()` / `notify_one()` / `notify_all()` |
| 锁粒度 | 方法级或块级 | 任意作用域 |
| 死锁预防 | 无内置 | `std::lock()` + `std::scoped_lock` 避免死锁 |

## 二、volatile — 完全不同！

**这是 Java 和 C++ 之间最大的陷阱之一**：

```java
// Java volatile:
// 1. 保证内存可见性（写后立即刷新到主存）
// 2. 禁止指令重排序
// 3. 不保证原子性（i++ 仍非原子）
volatile boolean running = true;
```

```cpp
// C++ volatile:
// 1. 仅禁止编译器优化（每次从内存读取，不缓存到寄存器）
// 2. 用于内存映射 I/O、信号处理
// 3. 不保证多线程可见性！
// 4. 不禁止 CPU 指令重排序！
// 5. 不保证原子性！
volatile int hardwareRegister;  // 仅用于硬件访问

// C++ 多线程正确做法 → std::atomic
std::atomic<bool> running = true;  // 这才是 Java volatile 的等价物
```

| 方面 | Java `volatile` | C++ `volatile` | C++ `std::atomic` |
|------|----------------|---------------|------------------|
| 内存可见性 | 保证 | 不保证 | 保证（可配内存序） |
| 禁止重排序 | 是 | 仅编译器，不禁止 CPU | 是（可配内存序） |
| 原子性 | 仅读写，不保证 RMW | 不保证 | 保证 |
| 用途 | 多线程标志位 | 硬件寄存器、信号处理 | 多线程共享数据 |

## 三、无内置线程池

```java
// Java — 标准库内置线程池
ExecutorService executor = Executors.newFixedThreadPool(4);
Future<Integer> future = executor.submit(() -> 42);
executor.shutdown();
```

C++ 标准库**没有线程池**。需要：
- 手动管理 `std::thread` 生命周期
- 使用第三方库（如 Intel TBB）
- 自己实现线程池
- C++26 可能引入 `std::execution`（sender/receiver 模型）

```cpp
// C++ — 手动创建线程
#include <thread>
#include <vector>

std::vector<std::thread> threads;
for (int i = 0; i < 4; ++i) {
    threads.emplace_back([] { /* work */ });
}
for (auto& t : threads) t.join();  // 手动等待
```

## 四、wait/notify → condition_variable

```java
// Java
synchronized (lock) {
    while (!condition) {
        lock.wait();       // 释放锁并等待
    }
    // 条件满足，继续执行
}

synchronized (lock) {
    lock.notify();         // 唤醒一个等待线程
    lock.notifyAll();      // 唤醒所有等待线程
}
```

```cpp
// C++
std::mutex mtx;
std::condition_variable cv;
bool ready = false;

// 等待端
{
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return ready; });  // 等价 while + wait
}

// 通知端
{
    std::lock_guard<std::mutex> lock(mtx);
    ready = true;
}
cv.notify_one();   // 通知可以放到锁外
cv.notify_all();

// 关键差异：必须用 unique_lock（不是 lock_guard），
// 因为 wait 需要临时解锁
```

| 概念 | Java | C++ |
|------|------|-----|
| 等待 | `obj.wait()` | `cv.wait(lock, predicate)` |
| 唤醒一个 | `obj.notify()` | `cv.notify_one()` |
| 唤醒全部 | `obj.notifyAll()` | `cv.notify_all()` |
| 锁类型 | 隐式（synchronized 锁的对象） | 显式 `std::unique_lock<std::mutex>` |
| 虚假唤醒 | JVM 保证处理？仍需 while | 必须用 predicate 版本或 while 循环 |

## 五、Future/Async 差异

```java
// Java
CompletableFuture.supplyAsync(() -> compute())
    .thenApply(result -> result * 2)
    .thenAccept(System.out::println);
```

```cpp
// C++
auto future = std::async(std::launch::async, compute);
auto result = future.get();  // 阻塞等待

// C++ 没有 thenApply/thenAccept 等组合子（C++20 仍无）
// 需要自己编排，或用 folly、boost::future 等库
```

## 六、快速参考卡

| 场景 | Java 做法 | C++ 做法 |
|------|----------|---------|
| 创建线程 | `new Thread(r).start()` | `std::thread t(func); t.join();` |
| 互斥锁 | `synchronized` | `std::mutex` + `std::lock_guard` |
| 多线程可见标志 | `volatile` | `std::atomic<bool>` |
| 硬件寄存器访问 | （无对应） | `volatile` |
| 等待条件 | `wait()` / `notify()` | `std::condition_variable` |
| 异步任务 | `ExecutorService` / `CompletableFuture` | `std::async` / `std::future` |
| 只调用一次 | `static` 初始化（类加载时） | `std::call_once` / Meyer's Singleton |
| 线程局部存储 | `ThreadLocal<T>` | `thread_local` 关键字 |
| 线程池 | `Executors.newFixedThreadPool(n)` | 无标准库，需自己实现或用第三方 |
| 原子计数器 | `AtomicInteger` | `std::atomic<int>` |
| 读写锁 | `ReentrantReadWriteLock` | `std::shared_mutex`（C++17） |
| 信号量 | `Semaphore` | `std::counting_semaphore`（C++20） |
| 内存顺序 | 无（JMM 隐式保证） | `memory_order_relaxed/acquire/release/seq_cst` |
