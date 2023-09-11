# 定時器

```cpp
#pragma once
#include <iostream>
#include <memory>
#include <functional>
#include <list>
#include <time.h>
using namespace std;

class TimerNode {
public:
    shared_ptr<function<void()>> m_pFunc;
    time_t endTime;

    TimerNode(int delay, shared_ptr<function<void()>> pFunc) :
        m_pFunc(pFunc), endTime(time(0) + delay) {}
    ~TimerNode() {}

    bool operator<(const TimerNode& rhs) const {
        return endTime < rhs.endTime;
    }

    bool operator==(int id) const {
        return reinterpret_cast<int>(m_pFunc.get()) == id;
    }

    int getID() const {
        return reinterpret_cast<int>(m_pFunc.get());
    }
};

class ListTimer {
    friend TimerNode;
    list<TimerNode> timeList;
public:
    ListTimer() {}
    ~ListTimer() {}

    int add(int delay, shared_ptr<function<void()>> cb) {
        TimerNode timer(delay, cb);
        timeList.push_back(timer);
        timeList.sort();
        return timer.getID();
    }

    void CancelTimer(int id) {
        auto it = find(timeList.begin(), timeList.end(), id);
        if (it != timeList.end())
            timeList.erase(it);
    }

    void update(int time) {
        list<TimerNode> item;

        for (auto it = timeList.begin(); it != timeList.end(); ++it)
        {
            TimerNode& node = *it;
            if (time < node.endTime) break;

            item.emplace_back(node);
            it = timeList.erase(it);
        }

        while (!item.empty())
        {
            auto func = *item.front().m_pFunc.get();
            item.pop_front();
            func();
        }
    }
};
```
