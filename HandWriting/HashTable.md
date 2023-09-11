# 哈希表

```cpp
#include <vector>
#include <list>

using namespace std;

int MyHash(const int& key, const int& tableSize) 
{
    return key % tableSize;
}

template <typename T>
class HashTable {
    vector<list<T>> m_table;

    int Hash(const T& obj) const {
        return MyHash(obj, m_table.size());
    }

public:
    HashTable(int size = 31) {
        m_table.reserve(size);
        m_table.resize(size);
    }

    ~HashTable() {}

    bool count(const T& obj) {
        const list<T>& slot = m_table[Hash(obj)];
        auto it = slot.cbegin();
        while (it != slot.cend() && *it != obj)
            ++it;
        return it != slot.cend();
    }

    bool insert(const T& obj) {
        if (count(obj)) return false;
        m_table[Hash(obj)].push_front(obj);
        return true;
    }

    bool remove(const T& obj) {
        list<T>& slot = m_table[Hash(obj)];
        auto it = find(slot.begin(), slot.end(), obj);
        if (it == slot.end())
            return false;
        slot.erase(it);
        return true;
    }
};

void testHastTable()
{
    HashTable<int> hash;

    cout << "Insert 1, 2, 3" << endl;
    hash.insert(1);
    hash.insert(2);
    hash.insert(3);

    cout << "Hash has 1 ? " << (hash.count(1) ? "Yes" : "No") << endl;
    cout << "Remove 1" << endl;
    hash.remove(1);
    cout << "Hash has 1 ? " << (hash.count(1) ? "Yes" : "No") << endl;
}
```
