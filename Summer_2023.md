# 2023年夏 学习logs

## 5月阶段目标

1) LeetCode刷题数到达400；

2) 完成 `GAMES101` 的复习笔记；

3) 学习Vulkan，并跟做 `GAMES106` 作业；

## 2023.5.4

1) LeetCode 刷题：

    中等题：`#2106.摘水果（滑动窗口）`

## 2023.5.5

1) LeetCode 刷题：

    简单题：`#2432`

    中等题：`#785.判断二分图（BFS）`, `#210.课程表II（拓扑排序）`, `#684.冗余连接（并查集）`

### [`#210` 拓扑排序](https://leetcode.cn/problems/course-schedule-ii/)

> ***深度优先搜索***

对于图中的任意一个节点，它在搜索的过程中有三种状态，即：

- $\lceil$ 未搜索 $\rfloor$：还没有搜索到这个节点；
- $\lceil$ 搜索中 $\rfloor$：搜索过这个节点，但还没有回溯到该节点，即该节点还没有入栈，还有相邻的节点没有搜索完成；
- $\lceil$ 已完成 $\rfloor$：搜索过并且回溯过这个节点，即该节点已经入栈，并且所有该节点的相邻节点都出现在栈的更底部的位置，满足拓扑排序的要求。

通过上述的三种状态，就可以给出使用深度优先搜索得到拓扑排序的算法流程。在每一轮的搜索搜索开始时，任取一个 $\lceil$ 未搜索 $\rfloor$ 的节点开始进行深度优先搜索。

- 将当前搜索的节点 $u$ 标记为 $\lceil$ 搜索中 $\rfloor$, 遍历该节点的每一个相邻节点 $v$:
  - 如果 $v$ 为 $\lceil$ 未搜索 $\rfloor$，那么开始搜索 $v$， 待搜索完成回溯到 $u$；
  - 如果 $v$ 为 $\lceil$ 搜索中 $\rfloor$，那么就找到了图中的一个环，因此是不存在拓扑排序的；
  - 如果 $v$ 为 $\lceil$ 已完成 $\rfloor$，那么说明 $v$ 已经在栈中了，而 $u$ 还不在栈中，因此 $u$ 无论何时入栈都不会影响到 $(u, v)$ 之前的拓扑关系，以及不用进行任何操作；
- 当 $u$ 所有相邻节点都为 $\lceil$ 已完成 $\rfloor$ 时，将 $u$ 放入栈中，并将其标记为 $\lceil$ 已完成 $\rfloor$。

在整个深度优先搜索的过程结束后，如果我们没有找到图中的环，那么栈中存储这所有的 $n$ 个节点，从栈顶到栈底的顺序即为一种拓扑排序。

> ***广度优先搜索（`Kahn 算法`）***

使用一个队列来进行广度优先搜索。开始时，所有入度为 $0$ 的节点都被放入队列中，它们就是可以作为拓扑排序最前面的节点，并且它们之间的相对顺序是无关紧要的。

在广度优先搜索的每一步中，取出队首的节点 $u$：

- 将 $u$ 放入答案中；
- 将 $u$ 所有相邻节点的入度减少 $1$。如果某个相邻节点 $v$ 的入度变为 $0$，那么就将 $v$ 放入队列中。

在广度优先搜索的过程结束后。如果答案中包含了这 $n$ 个节点（或所有节点入度均为0），那么我们就找到了一种拓扑排序，否则说明图中存在环，也就不存在拓扑排序了。

### [`#684` 并查集](https://leetcode.cn/problems/redundant-connection/)

> 并查集 $union \ find$, 或 $disjoint \ set$ 可以动态地连通两个点，并且可以非常快速地判断两个点是否连通。

假设存在 $n$ 个节点，先将所有节点的父亲标为自己；每次要连接节点 $i$ 和 $j$ 时，可以将 $i$ 的父亲标为 $j$；每次要查询两个节点是否相连时，可以查找 $i$ 和 $j$ 的祖先是否最终为同一个节点。

```cpp
class UF {
    vector<int> id, size;
public:
    UF(int n): id(n), size(n, 1) {
        iota(id.begin(), id.end(), 0);
    }

    int Find(int p) {
        while (p != id[p]) {
            id[p] = id[id[p]];  // 路径压缩
            p = id[p];
        }
        return p;
    }

    void Union(int p, int q) {
        // 加权 quick-union : 每次都把小树的根节点连接到大树的根节点
        int i = Find(p), j = Find(q);
        if (i == j) 
            return;
        if (size[i] < size[j]) {
            id[i] = j;
            size[j] += size[i];
        }
        else {
            id[j] = i;
            size[i] += size[j];
        }
    }

    bool isConnected(int p, int q) {
        return Find(p) == Find(q);
    }
};
```

## 2023.5.6

1) LeetCode 刷题：

    中等题：`#1419.数青蛙（模拟）`，`#146.LRU缓存（LRU）`

### [`#146` LRU缓存](https://leetcode.cn/problems/lru-cache/)

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

***复杂度分析***

- 时间复杂度：对于 `put` 和 `get` 都是 $O(1)$。
- 空间复杂度： $O(capacity)$，因为哈希表和双向链表最多存储 $capacity + 1$

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

## 2023.5.7

1) LeetCode 刷题：

    中等题：`#1010`

2) LeetCode 周赛：

   按时完成 `3` 题：`#2670`，`#2671`，`#2672`

   未按时完成：`#2673`

## 2023.5.8

1) LeetCode 刷题：

    中等题：`#1263.推箱子（0-1 BFS）`，`#752.打开转盘锁（A*）`

    动态规划专项：`#712.两个字符串的最小ASCII删除和`

### [`#146` 双端队列 BFS](https://leetcode.cn/problems/minimum-moves-to-move-a-box-to-their-target-location/)

***双端队列 BFS***（[OI-wiki](https://oi-wiki.org/graph/bfs/#%E5%8F%8C%E7%AB%AF%E9%98%9F%E5%88%97-bfs)）又称 0-1 BFS，通常用于处理边权值为可能有，也可能没有（由于 BFS 适用于权值为 1 的图，所以一般权值是 0 或 1），或者能够转化为这种边权值的最短路问题。

例如在走迷宫问题中，你可以花 1 个金币走 5 步，也可以不花金币走 1 步，这就可以用 0-1 BFS 解决。

一般情况下，我们把没有权值的边扩展到的点放到队首，有权值的边扩展到的点放到队尾。这样即可保证像普通 BFS 一样整个队列队首到队尾权值单调不下降。

伪代码实现如下：

```cpp
while (队列不为空) {
  int u = 队首;
  弹出队首;
  for (枚举 u 的邻居) {
    更新数据
    if (...)
      添加到队首;
    else
      添加到队尾;
  }
}
```

### [`#752` A*](https://leetcode.cn/problems/open-the-lock/)

$A^*$ 搜索算法（[OI-wiki](https://oi-wiki.org/search/astar/)）,简称 $A^*$ 算法，是一种在图形平面上，对于有多个节点的路径求出最低通过成本的算法。它属于图遍历和最佳优先搜索算法，亦是 BFS 的改进。

在 $A^*$ 算法中，需要使用四个距离函数 $F(x)$，$G(x)$，$H(x)$，$H^*(x)$，其中 $F(x)$，$G(x)$，$H(x)$ 是可以求出的，而 $H^*(x)$ 是无法求出的，需要用 $H(x)$ 近似 $H^*(x)$。设起点为 $s$，终点为 $t$，这些距离函数的意义如下：

- $G(x)$ 表示从起点 $s$ 到节点 $x$ 的 $\lceil$ 实际 $\rfloor$ 路径长度，其中 $G(x)$ 并不一定是最短的；
- $H(x)$ 表示从节点 $x$ 到终点 $t$ 的 $\lceil$ 估计 $\rfloor$ 路径长度，成为**启发函数**；
- $H^*(x)$ 表示从节点 $x$ 到终点 $t$ 的 $\lceil$ 实际 $\rfloor$ 路径长度，这是在广度优先搜索的过程中无法求出的，需要用 $H(x)$ 来近似 $H^*(x)$；
- $F(x)$ 满足 $F(x)=G(x) + H(x)$，即从起点 $s$ 到终点 $t$ 的 $\lceil$ 估计 $\rfloor$ 路径长度。通常总是挑选出最小的 $F(x)$ 对应的 $x$ 进行搜索，因此 $A^*$ 算法需要借助**优先队列**来实现。

## 2023.5.9

1) LeetCode 刷题：

    每日进度（6 / 5）

    简单题：`#2437`

    动态规划专项：`#1458.两个子序列的最大点积`，`#97.交错字符串`，`#518.零钱兑换II`，`#673.最长递增子序列的个数`，`#1964.找出到每个位置为止最长的有效障碍赛跑路线`

## 2023.5.10

1) LeetCode 刷题：

    每日进度（5 / 5）

    简单题：`#35.搜索插入位置（二分）`

    中等题：`#1015`

    动态规划专项：`#1671.得到山形数组的最少删除次数`，`#354.俄罗斯套娃信封问题`，`#1626.无矛盾的最佳球队`

2) 啃 Vulkan Tutorial

## 2023.5.11

1) LeetCode 刷题：

    每日进度（6 / 5）

    简单题：`#374.猜数字大小（二分）`

    中等题：`#1016.子串能表示从1到N数字的二进制串（滑动窗口）`

    动态规划专项：
    - 状态机DP：`#1911.最大子序列交替和`，
    - 区间DP：`#516.最长回文子序列`，`#.375.猜数字大小II`，`#1312.让字符串成为回文串的最少插入次数`
    - 复习：状态机DP（`#121`，`#122`，`#123`，`#188`，`#309`，`#714`， `#1039`）

2) 啃 Vulkan Examples

### [`#1016` `st::bitset`](https://leetcode.cn/problems/binary-string-with-substrings-representing-1-to-n/solution/san-chong-suan-fa-cong-bao-li-dao-you-hu-nmtq/)

which defined in header `<bitset>`

```cpp
template<std::size_t N>
class bitset;
```

The class template [bitset](https://en.cppreference.com/w/cpp/utility/bitset) represents a fixed-size sequence of N bits. Bitsets can be manipulated by standard logic operators and converted to and from strings and integers. For the purpose of the string representation and of naming directions for shift operations, the sequence is thought of as having its lowest indexed elements at the right, as in the binary representation of integers.

### [`#10147` 区间DP](https://loj.ac/p/10147)

区间类动态规划是线性动态规划的扩展，它在分阶段地划分问题时，与阶段中元素出现的顺序和由前一阶段的哪些元素合并而来有很大的关系。

令状态 $f(i, j)$ 表示将下标位置 $i$ 到 $j$ 的所有元素合并能获得的价值的最大值，那么 $f(i, j) = max{f(i, k) + f(k + 1, j) + cost}$，$cost$ 为将这两组元素合并起来的代价。

区间 DP 有以下特点：

- 合并：即将两个或多个部分进行整合，当然也可以反过来；
- 特征：能将问题分解为能两两合并的形式；
- 求解：对整个问题设最优值，枚举合并点，将问题分解为左右两个部分，最后合并两个部分的最优值得到原问题的最优值。

## 2023.5.12

1) LeetCode 刷题：

    每日进度（5 / 5）

    困难题：`#1330.翻转子数组得到最大的数组值（数学）`

    动态规划专项：
    - 区间DP：`#1547.切棍子的最小成本`
    - 复习：区间DP（`#1000`，`#312`）
    - 复习：多重背包（`#2585`）

    二叉树专项：
    - new：`#111.二叉树的最小深度`，`#129.求根节点到叶节点数字之和`，`#1026.节点与其祖先之间的最大差值`
    - 复习：`#104`，`#112`，`#113`，`#257`

2) 啃 Vulkan Examples
