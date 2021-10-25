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
};


#endif //TEST_THREADPOOL_H
