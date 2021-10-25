//
// Created by LLH on 2021/10/25.
//

#ifndef TEST_READWRITELOCK_H
#define TEST_READWRITELOCK_H
class readWriteLock {
private:
    std::mutex readMtx;
    std::mutex writeMtx;
    int readCnt; // 已加读锁个数
public:
    readWriteLock() : readCnt(0) {}

    void readLock() {
        readMtx.lock();
        if (++readCnt == 1) {
            writeMtx.lock();  // 存在线程读操作时，写加锁（只加一次）
        }
        readMtx.unlock();
    }

    void readUnlock() {
        readMtx.lock();
        if (--readCnt == 0) { // 没有线程读操作时，释放写锁
            writeMtx.unlock();
        }
        readMtx.unlock();
    }

    void writeLock() {
        writeMtx.lock();
    }

    void writeUnlock() {
        writeMtx.unlock();
    }
};
#endif //TEST_READWRITELOCK_H
