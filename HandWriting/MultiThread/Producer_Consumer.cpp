#include <iostream>
#include <thread>
#include <condition_variable>
#include <list>
#include <iomanip>
#include <vector>

//using namespace std::literals::chrono_literals;

std::mutex mtx;
std::condition_variable cond;

const int MAX_QUEUE_LENGTH = 10;
const int TOTAL_TEST_NUMBER = 100;
int id = 1;

std::list<int> arr;

void print_buffer()
{
    std::cout << "queue [" << arr.size() << "] :";
    for (int i : arr)
        std::cout << std::setw(3) << i << " ";
    std::cout << "\n\n";
}

void producer()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, []() {return arr.size() < MAX_QUEUE_LENGTH; });
        if (id > TOTAL_TEST_NUMBER) break;
        
        std::cout << "producer " << std::this_thread::get_id() << " is produce " << id << std::endl;
        arr.push_back(id++);
        print_buffer();

        cond.notify_all();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer()
{
    while (1)
    {
        std::unique_lock<std::mutex> lock(mtx);
        cond.wait(lock, []() { return arr.size() > 0; });
        if (id > TOTAL_TEST_NUMBER && arr.empty()) break;

        std::cout << "consumer " << std::this_thread::get_id() << " is consumer " << arr.front() << std::endl;
        arr.pop_front();
        print_buffer();

        cond.notify_all();
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int main() {

    std::vector<std::thread> cons, prod;
    for (int i = 0; i < 10; ++i)
    {
        prod.emplace_back(producer);
        prod[i].detach();
    }
    for (int i = 0; i < 10; ++i)
    {
        cons.emplace_back(consumer);
        cons[i].detach();
    }

    std::cin.get();

    return 0;
}
