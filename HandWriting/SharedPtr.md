# 智能指针

经历过很多面试都会问到 `shared_ptr`, 它确实能涉及到非常多的知识点, 从基础的裸指针, 引用计数, RAII, 到智能指针, 再到面向对象徒手写一个 `class`, 一系列构造函数, 运算符重载, 或者再回到使用 `shared_ptr` 可能出现的问题以及解决方案, 有太多可以问的了.

## 基础构造函数

首先是写一个基础的框架。一个 `shared_ptr` 首先是一个类，因此我们需要先写出基础的成员变量、构造函数。

自然的，它需要一个模板来兼容指向不同类型的指针，因此这里我们用模板来定义其在堆中存储的内容。

同时， `shared_ptr` 的一大特性就是引用计数，一个好的保存计数器的方法是将其存在动态内存中。当创建对象时，我们分配一个新的计数器；当拷贝或者赋值对象时，我们拷贝指向计数器的指针。这样，副本和原对象都会指向相同的计数器。

于是有：

```cpp
template<typename T>
class MyShared_Ptr {
public:
    MyShared_Ptr() : m_ptr(nullptr), use(nullptr) {}

    MyShared_Ptr(T* ptr)
        : m_ptr(ptr), use(new size_t(1)) {}
    
private:
    T* m_ptr;
    std::size_t* use;
};
```

### 析构函数

析构函数不能无条件的 `delete ps` ，因为可能还有其他对象指向这块内存。析构函数应该递减引用计数，指出共享 `T` 的对象少了一个。如果计数器变为 `0` ，则析构函数释放 `m_ptr` 和 `use` 指向的内存。

```cpp
    ~MyShared_Ptr()
    {
        if (m_ptr && --*use == 0)
        {
            delete m_ptr;
            delete use;
        }
    }
```

## 复制语义函数

### 拷贝构造函数

当拷贝或赋值一个新的对象时，我们希望副本和原对象都指向相同的 `T`。此外，在进行拷贝时，还会递增该 `T` 相关联的计数器。

```cpp
    MyShared_Ptr(const MyShared_Ptr& p)
        :m_ptr(p.m_ptr), use(p.use) { ++* use; }
```

### 拷贝赋值运算符

根据三、五法则我们可以知道，拷贝赋值运算符执行类似拷贝构造函数和析构函数的工作。即，它必须递增右侧运算对象的引用计数，并递减左侧运算对象的引用计数，在必要时释放使用的内存。

此外，赋值运算符必须处理自赋值。我们通过先递增 `rhs` 中的计数然后再递减左侧运算对象中的计数来实现这一点。通过这种方法，当两个对象相同时，在我们检查 `m_ptr` 和 `use` 是否应该释放之前，计数器就已经被递增过了。

```cpp
    MyShared_Ptr& operator=(const MyShared_Ptr& rhs)
    {
        ++* rhs.use;
        if (m_ptr && -- * use == 0)
        {
            delete m_ptr;
            delete use;
        }
        m_ptr = rhs.m_ptr;
        use = rhs.use;
        return *this;
    }
```

## 交换操作

除了定义拷贝控制成员，管理资源的类通常还定义一个名为 `swap` 的函数。与拷贝控制成员不同，`swap` 并不是必要的。但是对于分配了资源的类，定义 `swap` 可能是一种很重要的优化手段。

```cpp
    void swap(MyShared_Ptr& rhs)
    {
        using std::swap;
        swap(m_ptr, rhs.m_ptr);
        swap(use, rhs.use);
    }
```

### 在赋值运算符中使用 `swap`

定义 `swap` 的类通常用 `swap` 来定义它们的赋值运算符。这些运算符使用了一种名为 ***拷贝并交换（copy and swap）*** 的技术。

```cpp
    MyShared_Ptr& operator=(const MyShared_Ptr& rhs)
    {
        MyShared_Ptr(rhs).swap(*this);
        return *this;
    }
```

在这个版本的赋值运算符中，我们构造了一个 `MyShared_Ptr` 的临时对象。然后调用 `swap` 来交换临时对象和 `*this` 中的数据成员。这个调用将左侧运算对象中原来保存的指针存入临时对象中，并将 `rhs` 中原来的指针存入 `*this` 中。因此，在 `swap` 调用之后， `*this` 中的指针成员将指向新分配的 `T` --右侧运算对象 `rhs` 中 `T` 的一个副本。

当赋值运算符结束时，临时对象被销毁， `MyShared_Ptr` 的析构函数将被执行。此析构函数 delete 临时对象现在指向的内存，即，释放掉左侧运算对象中原来的内存。

这个技术的有趣之处是它自动处理了自赋值情况且天然就是异常安全的。

## 移动语义

接着我们写移动构造函数（move constructor）和移动赋值运算符（move assignment operator）。这两个成员类似对应的拷贝操作，但它们从给定对象“窃取”资源而不是拷贝资源。

### 移动构造函数

类似拷贝构造函数，移动构造函数的第一个参数是该类类型的一个引用。不同于拷贝构造函数的是，这个引用从参数在移动构造函数中是一个右值引用。

```cpp
    MyShared_Ptr(MyShared_Ptr&& p) noexcept
        :m_ptr(p.m_ptr), use(p.use)
    {
        p.m_ptr = nullptr;
        p.use = nullptr;
    }
```

由于移动操作“窃取”资源，它通常不分配任何资源。因此，移动操作通常不会抛出任何异常。因此在移动构造函数中需要指明 `noexcept`。

### 移动赋值运算符

移动赋值运算符执行与析构函数和移动构造函数相同的工作。与移动构造函数一样，如果我们的移动赋值运算符不抛出任何异常，我们就应该将它标记为 `noexcept`。类似拷贝赋值运算符，移动赋值运算符必须正确处理自赋值。

```cpp
    MyShared_Ptr& operator=(MyShared_Ptr&& rhs) noexcept
    {
        MyShared_Ptr(std::move(rhs)).swap(*this);
        return *this;
    }
```

这里函数体中第一行代码包含了三个操作：

- 用移动构造函数创建出一个新的 `MyShared_Ptr` 临时对象，此时原 `rhs` 的内容被清除了；
- 再和 `this` 交换指针和引用计数；
- 因为 `this` 的内容被交换到了临时对象里，临时对象析构之后，原 `this` 指向的引用计数 -1；

## 行为像指针的类

对于一个行为像指针的类，自然需要重载一些带有强烈指针特性的运算符。

```cpp
    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    explicit operator bool() const { return m_ptr; }
```

以及智能指针需要的操作。

```cpp
    T* get() const { return m_ptr; }
    bool unique() const { return *use == 1; }
    std::size_t use_count() const { return *use; }
```

## 整体实现

```cpp
#include <iostream>

using std::cout; using std::endl;

template<typename T>
class MyShared_Ptr {
public:
    MyShared_Ptr() : m_ptr(nullptr), use(nullptr) {}

    MyShared_Ptr(T* ptr)
        : m_ptr(ptr), use(new size_t(1))
    {
        cout << *m_ptr << " use count: " << *use << endl;
    }

    MyShared_Ptr(const MyShared_Ptr& p)
        :m_ptr(p.m_ptr), use(p.use)
    {
        ++* use;
        cout << *m_ptr << " use count: " << *use << endl;
    }

    MyShared_Ptr(MyShared_Ptr&& p) noexcept
        :m_ptr(p.m_ptr), use(p.use)
    {
        p.m_ptr = nullptr;
        p.use = nullptr;
        cout << *m_ptr << " Moved! Use count: " << *use << endl;
    }

    ~MyShared_Ptr()
    {
        if (!m_ptr) return;
        cout << *m_ptr << " use count: " << *use - 1 << endl;
        if (-- * use == 0)
        {
            cout << *m_ptr << " destructed" << endl;
            delete m_ptr;
            delete use;
        }
    }

    MyShared_Ptr& operator=(const MyShared_Ptr& rhs)
    {
        cout << "Assignment Operator" << endl;
        ++* rhs.use;
        if (m_ptr && -- * use == 0)
        {
            cout << *m_ptr << " destructed" << endl;
            delete m_ptr;
            delete use;
        }
        m_ptr = rhs.m_ptr;
        use = rhs.use;
        return *this;
    }

    //MyShared_Ptr& operator=(const MyShared_Ptr& rhs)
    //{
    //    cout << "Assignment operator" << endl;
    //    MyShared_Ptr(rhs).swap(*this);
    //    return *this;
    //}

    MyShared_Ptr& operator=(MyShared_Ptr&& rhs) noexcept
    {
        cout << "Move Assignment Operator" << endl;
        MyShared_Ptr(std::move(rhs)).swap(*this);
        return *this;
    }

    void swap(MyShared_Ptr& rhs)
    {
        using std::swap;
        swap(m_ptr, rhs.m_ptr);
        swap(use, rhs.use);
    }

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    explicit operator bool() const { return m_ptr; }

    T* get() const { return m_ptr; }
    bool unique() const { return *use == 1; }
    std::size_t use_count() const { return *use; }

private:
    T* m_ptr;
    std::size_t* use;
};

int main()
{
    {
        MyShared_Ptr<double> test1(new double(10086.));
        {
            cout << "---------- 拷贝构造函数 ----------\n";
            auto test2 = test1;
            cout << *test1 << ", " << *test2 << endl;

            cout << "---------- 移动构造函数 ----------\n";
            auto test3 = std::move(test2);

            cout << "---------- 移动赋值运算符 ----------\n";
            test3 = std::move(test3);
            
            cout << "---------- 测试 ----------\n";
            if (test3)
                cout << "true" << endl;
            else
                cout << "false" << endl;
            cout << *test1 << ", " << *test3 << endl;
        }
        cout << *test1 << endl;
    }

    std::cin.get();
}

```

## 循环引用问题

由于面试中经常会被问到 `shared_ptr` 的循环引用问题，因此这里对此做一点补充。

`shared_ptr` 的循环引用问题是什么? 怎么体现出来的？

循环引用就是用 `shared_ptr` 指向的两个实例都存着另一方的 `shared_ptr`, 你中有我我中有你, 导致资源无法回收. 直接看下面代码案例。

```cpp
#include <iostream>
#include <memory>
class A
{
public:
    std::shared_ptr<A> m_ptr;
    void set_ptr(std::shared_ptr<A> p) {
        m_ptr = p;
    }

    ~A()
    {
        std::cout << "dtor called" << std::endl;
    }
};

int main()
{
    std::shared_ptr<A> p1 = std::make_shared<A>();
    std::shared_ptr<A> p2 = std::make_shared<A>();

    std::cout << "p1.use_count():" << p1.use_count() << std::endl;  // p1 ref count == 1
    std::cout << "p2.use_count():" << p2.use_count() << std::endl;  // p2 ref count == 1

    p1->set_ptr(p2);
    p2->set_ptr(p1);

    // 我们把指针存在了另一个实例的成员里，所以引用计数都是 2
    std::cout << "p1.use_count():" << p1.use_count() << std::endl;  // p1 ref count == 2
    std::cout << "p2.use_count():" << p2.use_count() << std::endl;  // p2 ref count == 2

    return 0;
}

// p1 ref count == 1
// p2 ref count == 1
// 这个区域运行结束后,表层的p1,p2的引用计数都会-1,
// 但是因为都互相存着对方的指针,导致无法调用析构函数
// 所以成员变量一直滞留,这就是循环引用问题
```
