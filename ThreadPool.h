//
// Created by LLH on 2021/10/25.
//
#ifndef TEST_THREADPOOL_H
#define TEST_THREADPOOL_H

#include <vector>
#include <queue>
#include <atomic>
#include <future>
#include <condition_variable>
#include <thread>
#include <functional>
#include <stdexcept>
#include<memory>
#define  THREADPOOL_MAX_NUM 16
using namespace std;
class ThreadPool {
private:
    using Task = function<void()>;
    vector<thread> MyPool;//线程池
    queue<Task> MyTaskQueue;//任务队列
    mutex MyLock;//互斥锁
    condition_variable ConLock;//条件锁
    atomic<bool> Running{true};//运行状态
    atomic<uint16_t> NoTaskNum{0};
public:
    explicit ThreadPool(uint16_t size = 2);

    ~ThreadPool();

    uint16_t FreeThreadNum();

    uint32_t CountThreadNum();

    void addThread(uint16_t size);

    template<typename F, typename...Args>
    auto commit(F &&f, Args &&... args) -> future<decltype(f(args...))> {
        using RetType = decltype(f(args...)); // typename std::result_of<F(Args...)>::type, 函数 f 的返回值类型
        auto task = make_shared<packaged_task<RetType()>>(
                bind(forward<F>(f), forward<Args>(args)...)
        ); // 把函数入口及参数,打包(绑定)
        future<RetType> future = task->get_future();
        {    // 添加任务到队列
            lock_guard<std::mutex> lock{MyLock};//对当前块的语句加锁  lock_guard 是 mutex 的 stack 封装类，构造的时候 lock()，析构的时候 unlock()
            MyTaskQueue.emplace([task]() { // push(Task{...}) 放到队列后面
                (*task)();
            });
        }

        if (NoTaskNum < 1 && MyPool.size() < THREADPOOL_MAX_NUM) {
            addThread(1);
        }
        ConLock.notify_one(); // 唤醒一个线程执行
        return future;
    }
};

ThreadPool::ThreadPool(uint16_t size) {
    this->addThread(size);
}

ThreadPool::~ThreadPool() {
    Running = false;
    ConLock.notify_all();
    for (thread &th: MyPool) {
        if (th.joinable()) {
            th.join();
        }
    }
}

uint16_t ThreadPool::FreeThreadNum() {
    return NoTaskNum;
}

uint32_t ThreadPool::CountThreadNum() {
    return MyPool.size();
}

void ThreadPool::addThread(uint16_t size) {
    for (; MyPool.size() < THREADPOOL_MAX_NUM && size > 0; --size) {
        MyPool.emplace_back([this]() {
            while (Running) {
                unique_lock<mutex> templock(MyLock);
                ConLock.wait(templock, [this]() {
                    return !Running || !MyTaskQueue.empty();
                });
                if (!Running && MyTaskQueue.empty()) {
                    return;
                }
                Task task(move(MyTaskQueue.front()));
                MyTaskQueue.pop();
                templock.unlock();
                --NoTaskNum;
                task();
                ++NoTaskNum;
            }
        });
        ++NoTaskNum;
    }
}
#endif //TEST_THREADPOOL_H
