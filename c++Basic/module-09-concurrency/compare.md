# 模块 9: 并发编程 — Java vs C++

## 核心差异

| 维度 | Java | C++ |
|------|------|-----|
| 线程创建 | `new Thread(runnable).start()` | `std::thread t(func, args)` |
| 同步 | `synchronized` 关键字/块 | `std::mutex` + `std::lock_guard` |
| 原子操作 | `AtomicInteger`, `AtomicReference` | `std::atomic<T>` |
| 异步任务 | `CompletableFuture<T>` | `std::async` / `std::future<T>` |
| 线程池 | `ExecutorService` | C++17 没有标准线程池，需第三方或手写 |
| 条件变量 | `wait()/notify()` | `std::condition_variable` |

## 知识点: std::thread

| 场景 | Java | C++ |
|------|------|-----|
| 创建线程 | `new Thread(() -> {...}).start()` | `std::thread t([]{...});` |
| 等待线程 | `thread.join()` | `t.join()` |
| 分离线程 | 无 | `t.detach()` |

## 知识点: 互斥锁

| 场景 | Java | C++ |
|------|------|-----|
| 加锁 | `synchronized(obj) { }` | `std::lock_guard<std::mutex> lg(mtx);` |
| 更灵活的锁 | `ReentrantLock` | `std::unique_lock` (可延迟、可转移) |
| 死锁避免 | 手动排序 + tryLock | `std::lock(m1, m2)` 同时锁定 |

## 常见 Bug

- [ ] 忘记 join 或 detach → `std::terminate`
- [ ] 数据竞争（非原子操作的并发读写）
- [ ] 死锁（交叉获取多个锁）
- [ ] `std::mutex` 忘记 unlock → `lock_guard` 解决
