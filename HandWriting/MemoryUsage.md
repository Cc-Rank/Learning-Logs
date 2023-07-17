# 内存使用管理

包括重载 `operator new` 和 `operator delete` ，以及单例模式实现 `AllocationMetrics`。

```cpp
#include <iostream>
#include <string>
#include <memory>

using std::cout; using std::endl;

class AllocationMetrics {
    friend void* operator new(size_t size);
    friend void operator delete(void* mem, size_t size) noexcept;

public:
    AllocationMetrics(const AllocationMetrics&) = delete;

    static AllocationMetrics& GetInstance() { return s_AllocationMetrics; }

    uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
private:
    AllocationMetrics() {}
    uint32_t TotalAllocated = 0;
    uint32_t TotalFreed = 0;
    static AllocationMetrics s_AllocationMetrics;
};

AllocationMetrics AllocationMetrics::s_AllocationMetrics;

static void PrintMemoryUsage()
{
    std::cout << "Memory Usage: " << AllocationMetrics::GetInstance().CurrentUsage() << "bytes\n";
}

void* operator new (size_t size)
{
    //cout << "Allocating " << size << " bytes\n";
    AllocationMetrics::GetInstance().TotalAllocated += size;

    return malloc(size);
}

void operator delete(void* mem, size_t size) noexcept 
{
    //cout << "Free " << size << " bytes" << " memories\n";
    AllocationMetrics::GetInstance().TotalFreed += size;
    free(mem); 
}

int main()
{
    PrintMemoryUsage();
    {
        std::unique_ptr<float> test = std::make_unique<float>(1);
        PrintMemoryUsage();

        cout << sizeof(test) << endl;
    }
    PrintMemoryUsage();

    std::cin.get();
}
```
