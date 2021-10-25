#include "ThreadPool.h"
#include <iostream>
#include<future>

int myfun(int l) {
    return l + 3;
}

int main() {
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
    ThreadPool pool(4);
    future<int> k = pool.commit(myfun, 2);
    cout << k.get();
}
