# C++11 简易线程池

## 线程池

```cpp
#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <functional>

using namespace std;

class ThreadPool {
    typedef function<void()> Task;

    size_t n;
    vector<thread> threads;
    queue<Task> task;

    mutex mtx;
    condition_variable cond;
    bool done, isEmpty, isFull;

public:
    ThreadPool() : n(0), done(false), isEmpty(true), isFull(false) {}

    ThreadPool(size_t n) : n(n), done(false), isEmpty(true), isFull(false) {
        for (int i = 0; i < n; ++i)
            threads.push_back(thread(&ThreadPool::runTask, this));
    }

    ~ThreadPool() {
        done = true;
        cond.notify_all();
        for (int i = 0; i < n; ++i)
            if (threads[i].joinable())
                threads[i].join();
    }

    void addTask(const Task& t) {
        if (done) return;
        unique_lock<mutex> lock(mtx);

        cond.wait(lock, [this] { return !isFull; });

        task.push(t);
        if (task.size() == n)
            isFull = true;
        isEmpty = false;

        lock.unlock();
        cond.notify_one();
    }

    void runTask() {
        while (!done) {
            unique_lock<mutex> lock(mtx);
            if (isEmpty)
                cond.wait(lock);

            if (task.size()) {
                Task t;

                t = move(task.front());
                task.pop();
                if (task.empty())
                    isEmpty = true;
                isFull = false;
                lock.unlock();
                cond.notify_one();

                t();
            }
        }
    }
};
```

## 测试

```cpp
#include <iostream>
#include <vector>
#include <chrono>
#include "MyTP.hpp"

using namespace std;

void func(int i) {
    this_thread::sleep_for(chrono::microseconds(500));
    cout << "task finish" << "------>" << i << endl;
}

int main() 
{
    {
        ThreadPool p(10);
        int i = 10;

        while (i--) 
        {
            auto task = bind(func, i);
            p.addTask(task);
        }
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "-----End-----\n";

    cin.get();
}
```
