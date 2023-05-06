# [`#146` LRU缓存](https://leetcode.cn/problems/lru-cache/)

## 算法

LRU 缓存机制可以通过哈希表辅以双向链表实现，用一个哈希表和一个双向链表维护所有在缓存中的键值对。

- 双向链表按照被使用的顺序存储了这些键值对，靠近头部的键值对是最近使用的，而靠近尾部的键值对是最久未使用的。
- 哈希表通过缓存数据的键映射到其在双向链表中的位置。

这样一来，首先使用哈希表进行定位，找出缓存项在双向链表中的位置，随后将其移动到双向链表的头部，即可在 $O(1)$ 的时间内完成 `get` 或者 `put` 操作。具体方法如下：

- 对于 `get` 操作，首先判断 `key` 是否存在：
  - 如果 `key` 不存在，则返回 $-1$;
  - 如果 `key` 存在，则 `key` 对应的节点是最近被使用的节点。通过哈希表定位到该节点在双向链表中的位置，并将其移动到双向链表的头部，最后返回该节点的值。
- 对于 `put` 操作，首先判断 `key` 是否存在：
  - 如果 `key` 不存在，使用 `key` 和 `value` 创建一个新的节点，在双向链表的头部添加该节点，并将 `key` 和该节点添加进哈希表中。然后判断双向链表的节点数是否超出容量，如果超出容量，则删除双向链表的尾部节点，并删除哈希表中对应的项；
  - 如果 `key` 存在，则与 `get` 操作类似，先通过哈希表定位，再将对应的节点的值更新为 `value`，并将该节点移到双向链表的头部。

## 实现

```cpp
class LRUCache {
    unordered_map<int, list<pair<int, int>>::iterator> hash;
    list<pair<int, int>> cache;
    int size;
public:
    LRUCache(int capacity) : size(capacity) { }
    
    int get(int key) {
        auto it = hash.find(key);
        if (it == hash.end())
            return -1;
        cache.splice(cache.begin(), cache, it->second);
        return it->second->second;
    }
    
    void put(int key, int value) {
        auto it = hash.find(key);
        if (it != hash.end()) {
            it->second->second = value;
            cache.splice(cache.begin(), cache, it->second);
            return;
        }
        cache.insert(cache.begin(), make_pair(key, value));
        hash[key] = cache.begin();
        if (cache.size() > size) {
            hash.erase(cache.back().first);
            cache.pop_back();
        }
    }
};
```

## 复杂度分析

- 时间复杂度：对于 `put` 和 `get` 都是 $O(1)$。
- 空间复杂度： $O(capacity)$，因为哈希表和双向链表最多存储 $capacity + 1$

## 双向队列实现

如果面试官提出更高的要求，可以使用自己实现的双向队列来替代 `STL` 中的 `std::list`.

在双向链表中，使用一个**伪头部**（dummy head）和**伪尾部**（dummy tail）标记界限，这样在添加节点和删除节点的时候就不需要检查相邻的节点是否存在。

```cpp
struct DlinkedNode {
    int key, value;
    DlinkedNode* prev;
    DlinkedNode* next;
    DlinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DlinkedNode(int key, int value): key(key), value(value), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, DlinkedNode*> cache;
    DlinkedNode* head;
    DlinkedNode* tail;
    int size, capacity;
public:
    LRUCache(int capacity) : capacity(capacity), size(0) {
        head = new DlinkedNode();
        tail = new DlinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (!cache.count(key))
            return -1;
        DlinkedNode* node = cache[key];
        moveToHead(node);
        return node->value;
    }

    void put(int key, int value) {
        if (!cache.count(key)) {
            DlinkedNode* node = new DlinkedNode(key, value);
            cache[key] = node;
            addToHead(node);
            ++size;
            if (size > capacity)
            {
                DlinkedNode* removed = removeTail();
                cache.erase(removed->key);
                delete removed;
                --size;
            }
        }
        else {
            cache[key]->value = value;
            moveToHead(cache[key]);
        }
    }
    
    void addToHead(DlinkedNode* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(DlinkedNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void moveToHead(DlinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    DlinkedNode* removeTail() {
        DlinkedNode* node = tail->prev;
        removeNode(node);
        return node;
    }
};
```

## 线程安全LRUCache

TODO：
