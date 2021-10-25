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

class ThreadPool {
private:
    using Task = function<void()>;
    std::vector<std::thread> MyPool;//线程池
    std::queue<Task> MyTaskQueue;//任务队列
    std::mutex MyLock;//互斥锁
    std::condition_variable ConLock;//条件锁
    std::atomic<bool> Running{true};//运行状态
    std::atomic<uint16_t> MoTaskNum{0};
public:
    explicit ThreadPool(uint16_t);

    ~ThreadPool();

    int FreeThreadNum();

    int CountThreadNum();

    void addThread(uint16_t);

    template<typename F, typename...Args>
    auto commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>;
};

ThreadPool::ThreadPool(uint16_t size = 1) {

};

ThreadPool::~ThreadPool() {

}

int ThreadPool::FreeThreadNum() {

}

int ThreadPool::CountThreadNum() {
    return 0;
}

void ThreadPool::addThread(uint16_t) {

}

template<typename F, typename... Args>
auto ThreadPool::commit(F &&f, Args &&... args) -> std::future<decltype(f(args...))> {
    return nullptr;
};
#endif //TEST_THREADPOOL_H
