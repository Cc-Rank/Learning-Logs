# 堆的实现

**优先队列**（priority queue）可以在 $O(1)$ 时间内获得最大值，并且可以在 $O(\log n)$ 时间内取出最大值或插入任意值。其常常用 **堆**（heap）来实现。堆是一个完全二叉树，其每个节点的值总是大于等于子节点的值。

实际实现堆时，我们通常用一个长度为 $N + 1$ 的数组而不是指针建立一个树。这是因为堆是完全二叉树，所以用数组表示时，位置 $i$ 的节点的父节点位置一定为 $i / 2$，而它的两个子节点的位置又一定分别为 $2i$ 和 $2i + 1$。要注意的是，方便起见，我们通常不会使用下标为 $0$ 的位置。

```cpp
vector<int> heap;

int top() 
{
    return heap[1];
}

void push(int k) 
{
    heap.push_back(k);
    swim(heap.size() - 1);
}

void pop() 
{
    heap[1] = heap.back();
    heap.pop_back();
    sink(1, heap.size() - 1);
}

// 上浮
void swim(int pos) 
{
    while (pos > 1 && heap[pos / 2] < heap[pos]) 
    {
        swap(heap[pos / 2], heap[pos]);
        pos >>= 1;
    }
}

// 下沉
void sink(int pos, int N) 
{
    while (pos * 2 <= N)
    {
        int i = pos * 2;
        if (i < N && heap[i] < heap[i +1])
            ++i;
        if (heap[pos] >= heap[i])
            break;
        swap(heap[i], heap[pos]);
        pos = i;
    }
}

// 堆排序
void sort()
{
    int N = heap.size() - 1;
    for (int k = N / 2; k >= 1; --k)
        sink(k, N);
    while (N > 1)
    {
        swap(heap[1], heap[N--]);
        sink(1, N);
    }
}
```

当然，如果给定数组的下标是从 0 开始，也可以在原数组上完成堆的构建。

```cpp
void swim(int pos)
{
    while (pos > 0 && heap[(pos - 1) / 2] < heap[pos])
    {
        swap(heap[(pos - 1) / 2], heap[pos]);
        pos = (pos - 1) >> 1;
    }
}

void sink(vector<int>& heap, int pos, int n)
{
    while (pos * 2 < n)
    {
        int i = pos * 2 + 1;
        if (i < n && heap[i] < heap[i + 1])
            ++i;
        if (heap[pos] >= heap[i])
            break;
        swap(heap[i], heap[pos]);
        pos = i;
    }
}

void heapSort(vector<int>& heap, int k)
{
    int n = heap.size() - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        sink(heap, i, n);
    while (n > 0)
    {
        swap(heap[0], heap[n--]);
        sink(heap, 0, n);
    }
    // 当需要求前 k 个最大元素时。
    // int i = n;
    // while (i > n - k + 1)
    // {
    //     swap(heap[0], heap[i--]);
    //     sink(heap, 0, i);
    // }
}
```
