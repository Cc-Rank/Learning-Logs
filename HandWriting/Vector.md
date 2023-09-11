# Vector 动态管理内存类

作为 STL 中最常被使用的容器，`vector` 有着在程序运行时分配可变大小的内存空间这一特性。为了加深对于 C++ 中动态内存与拷贝控制的理解，亲自动手实现一个 `vector` 算得上是一个不错的开始。

STL 中的 `vector` 通常被认为有着许多可以优化的地方，因此许多公司会选择写一个自己的版本，如 EA 的 [*EASTL*](https://github.com/electronicarts/EASTL/blob/master/include/EASTL/vector.h)。此外，Cherno 在自己的 [*C++ Series*](https://www.youtube.com/watch?v=ryRf4Jh_YC0&list=PLlrATfBNZ98dudnM48yfGUldqGD0S4FFb&index=92) 中也给出一个简易的 `Vector` 实现教程。感兴趣的童鞋们也可以做一些参考。

言归正传，本文是基于《CPP Primer》实现的一个简化版的 STL `vector` 。因此引用了许多书中的内容。

## 基础框架

首先，`Vector` 类将其元素保存在连续内存中，并在每次添加新的元素时会检查是否有空间容纳更多的元素。如果有，成员函数会在下一个可用位置构造一个对象；如果没有可用空间，`Vector` 就会重新分配空间，然后将已有元素移动到新空间中，在新的空间添加新元素，并释放旧空间。本文将使用 [allocator](https://en.cppreference.com/w/cpp/memory/allocator) 来进行原始内存的分配，和对象的构造与销毁操作。

### 框架

每个 `Vector` 有三个指针成员指向其元素所使用的内存：

- `first`，指向分配的内存中的首元素；
- `last`，指向最后一个实际元素之后的位置；
- `end`，指向分配的内存末尾之后的位置；

除了这些，`Vector` 还有一个静态成员 `allocator<> alloc`，其会分配 `Vector` 所使用的内存，同时这里也定义了 3 个工具函数：

- `alloc_n_copy` 会分配内存，并拷贝一个给定范围中的元素；
- `free` 会销毁构造的元素并释放内存（奇怪的是 `std::vector` 并没有 `public` 这一功能）；
- `reallocate` 在内存用完时为 `Vector` 分配新内存；

有了 `Vector` 中的成员之后，接下来便是根据以往使用 `vector` 的习惯来给出我们 `Vector` 的简化版定义：

```cpp
#include <memory>

template<typename T>
class Vector
{
public:
    Vector() : m_first(nullptr), m_last(nullptr), m_end(nullptr) { }
    Vector(const Vector&);
    Vector& operator=(const Vector&);
    ~Vector();

    void push_back(const T&);
    size_t size() const { return m_last - m_first; }
    size_t capacity() const { return m_e nd - m_first; }

    T* begin() const { return m_first; }
    T* end() const { return m_last; }

private:
    static std::allocator<T> alloc;

    std::pair<T*, T*> alloc_n_copy(const T*, const T*);
    void free();
    void reallocate();

    T* m_first, * m_last, * m_end;
};

template<typename T>
std::allocator<T> Vector<T>::alloc;
```

### 成员

```cpp
#include <iostream>
#include <vector>
#include <memory>

using namespace std;

template <typename T>
class Vector {
public:
    Vector() : m_first(nullptr), m_last(nullptr), m_end(nullptr) {}
    Vector(const Vector&) {}

    ~Vector()
    {
        if (m_first)
        {
            for (auto p = m_last - 1; p != m_first; --p)
                alloc.destroy(p);
            alloc.deallocate(m_first, m_end - m_first);
        }
    }

    void push_back(const T& data)
    {
        if (size() == capacity())
        {
            size_t newCap = size() ? 2 * size() : 1;
            realloc(newCap);
        }

        alloc.construct(m_last++, data);
    }

    void push_back(T&& data)
    {
        if (size() == capacity())
        {
            size_t newCap = size() ? 2 * size() : 1;
            realloc(newCap);
        }

        alloc.construct(m_last++, std::move(data));
    }

    void resize(int n, const T& data) 
    {
        //if (n < size())
        //{
        //  for (auto p = m_last; p != m_first; )
        //      alloc.destroy(--p);
        //}
        //else
        //{

        //}
    }

    void reserve(size_t n)
    {
        if (n < capacity())
            return;

        realloc(n);
    }

    size_t size() const { return m_last - m_first; }
    size_t capacity() const { return m_end - m_first; }
    T* begin() const { return m_first; }
    T* end() const { return m_last; }
    const T& operator[](size_t idx) const { return *(m_first + idx); }
    T& operator[](size_t idx) { return *(m_first + idx); }

private:
    T* m_first, *m_last, * m_end;

    static allocator<T> alloc;

    void realloc(size_t newCap)
    {
        //size_t newCap = size() ? 2 * size() : 1;
        auto newData = alloc.allocate(newCap);

        T* target = newData;
        T* p = m_first;
        for (int i = 0; i < size(); ++i)
            alloc.construct(target++, std::move(*p++));

        if (m_first)
        {
            for (auto p = m_last; p != m_first; )
                alloc.destroy(--p);
            alloc.deallocate(m_first, m_end - m_first);
        }
        m_first = newData;
        m_last = target;
        m_end = m_first + newCap;
    }
};

template <typename T>
allocator<T> Vector<T>::alloc;

int main() {

    Vector<int> test;
    cout << test.size() << ", " << test.capacity() << endl;
    test.push_back(1);
    cout << test.size() << ", " << test.capacity() << endl;
    test.push_back(2);
    cout << test.size() << ", " << test.capacity() << endl;
    test.push_back(31);
    cout << test.size() << ", " << test.capacity() << endl;
    test.push_back(4);
    cout << test.size() << ", " << test.capacity() << endl;
    test.push_back(5);
    test.push_back(6);
    test.push_back(7);
    cout << test.size() << ", " << test.capacity() << endl;

    test.reserve(10);
    cout << test.size() << ", " << test.capacity() << endl;

    test[2] = 3;

    for (int i = 0; i < test.size(); ++i)
        cout << test[i] << endl;


    cin.get();
}
```
