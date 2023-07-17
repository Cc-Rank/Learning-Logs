# 智能指针

```cpp
#include <iostream>

using std::cout; using std::endl;

template<typename T>
class MyShared_Ptr {
public:
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

    ~MyShared_Ptr()
    {
        cout << *m_ptr << " use count: " << *use - 1 << endl;
        if (--*use == 0)
        {
            cout << *m_ptr << " destructed" << endl;
            delete m_ptr;
            delete use;
        }
    }

    MyShared_Ptr& operator=(const MyShared_Ptr& rhs)
    {
        cout << "assignment" << endl;
        ++* rhs.use;
        if (-- * use == 0)
        {
            cout << *m_ptr << " destructed" << endl;
            delete m_ptr;
            delete use;
        }
        m_ptr = rhs.m_ptr;
        use = rhs.use;
        return *this;
    }

    T& operator*() const { return *m_ptr; }
    T* operator->() const { return m_ptr; }
    explicit operator bool() const { return m_ptr; }

private:

    T* m_ptr;
    std::size_t* use;
};


int main()
{
    {
        MyShared_Ptr<double> test1(new double(1.));
        {
            auto test2 = test1;
            test2 = test2;

            cout << *test1 << ", " << *test2 << endl;
            if (test2)
                cout << "true" << endl;
        }
        cout << *test1 << endl;
    }

    std::cin.get();
}
```
