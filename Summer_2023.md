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

    中等题：`#1419.数青蛙（模拟）`, `#146.LRU缓存（LRU）`

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

若需要自己实现双向队列，参见也可以[自己实现](https://github.com/Cc-Rank/Learning-Logs/blob/main/HandWriting/LRUCache.md/#双向队列实现)来替换掉`std::list`。

## 2023.5.7

1) LeetCode 刷题：

    中等题：`#1010`

2) LeetCode 周赛：

   按时完成 `3` 题：`#2670`, `#2671`, `#2672`

   未按时完成：`#2673`

## 2023.5.8

1) LeetCode 刷题：

    中等题：`#1263.推箱子（0-1 BFS）`, `#752.打开转盘锁（A*）`

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

    动态规划专项：`#1458.两个子序列的最大点积`, `#97.交错字符串`, `#518.零钱兑换II`, `#673.最长递增子序列的个数`, `#1964.找出到每个位置为止最长的有效障碍赛跑路线`

## 2023.5.10

1) LeetCode 刷题：

    每日进度（5 / 5）

    简单题：`#35.搜索插入位置（二分）`

    中等题：`#1015`

    动态规划专项：`#1671.得到山形数组的最少删除次数`, `#354.俄罗斯套娃信封问题`, `#1626.无矛盾的最佳球队`

2) 啃 Vulkan Tutorial

## 2023.5.11

1) LeetCode 刷题：

    每日进度（6 / 5）

    简单题：`#374.猜数字大小（二分）`

    中等题：`#1016.子串能表示从1到N数字的二进制串（滑动窗口）`

    动态规划专项：
    - 状态机DP：`#1911.最大子序列交替和`
    - 区间DP：`#516.最长回文子序列`, `#.375.猜数字大小II`, `#1312.让字符串成为回文串的最少插入次数`
    - 复习：状态机DP（`#121`, `#122`, `#123`, `#188`, `#309`, `#714`,  `#1039`）

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
    - 复习：区间DP（`#1000`, `#312`）
    - 复习：多重背包（`#2585`）

    二叉树专项：
    - new：`#111.二叉树的最小深度`, `#129.求根节点到叶节点数字之和`, `#1026.节点与其祖先之间的最大差值`
    - 复习：`#104`, `#112`, `#113`, `#257`

2) 啃 Vulkan Examples

## 2023.5.13

1) LeetCode 刷题：

    每日进度（7 / 5）

    简单题：`#2441`

    二叉树专项：
    - new：`#100.相同的树`, `#199.二叉树的右视图`, `98.验证二叉搜索树`, `#230.二叉搜索树中第K小的元素`, `#501.二叉搜索树中的众数`, `#700.二叉搜索树中的搜索`
    - 复习：`#101`, `#110`, `#226`, `#530（Morris 中序遍历）`

## 2023.5.14

1) LeetCode 刷题：

    每日进度（9 / 5）

    中等题：`#1054`

2) LeetCode 双周赛：

   按时完成 `2` 题：`#2678`, `#2679`

   未按时完成：`#2680`, `#2681`

3) LeetCode 周赛：

   按时完成 `4` 题：`#2682`, `#2683`, `#2684`, `#2685`

## 2023.5.15

1) LeetCode 刷题：

    每日进度（5 / 5）

    中等题：`#1072（逆向思维）`, `#96.不同的二叉搜索树（DP）`

    二叉树专项：
    - new：`#102.二叉树的层序遍历`, `#103.二叉树的锯齿形层序遍历`, `#589.N叉树的前序遍历`
    - 复习：`#236`, `#235`, `#513`, `#144`

### [`#144` Morris 前序遍历](https://leetcode.cn/problems/binary-tree-preorder-traversal/solution/er-cha-shu-de-qian-xu-bian-li-by-leetcode-solution/)

> 有一种巧妙的方法可以在线性时间内，只占用常数空间来实现前序遍历。这种方法由 J. H. Morris 在 1979 年的论文 $\lceil$ Traversing Binary Trees Simply and Cheaply $\rfloor$ 中首次提出，因此被称为 Morris 遍历。

Morris 遍历的核心思想是利用树的大量空闲指针，实现空间开销的极限缩减。其前序遍历规则总结如下：

1. 如果当前节点的左子节点为空，将当前节点加入答案，并遍历当前节点的右子节点；
2. 如果当前节点的左子节点不为空，在当前节点的左子树中找到当前节点在中序遍历下的前驱节点：
   - 如果前驱节点的右子节点为空，将前驱节点的右子节点设置为当前节点。然后将当前节点加入答案，并将前驱节点的右子节点更新为当前节点。当前节点更新为当前节点的左子节点。
   - 如果前驱节点的右子节点为当前节点，将它的右子节点重新设为空。当前节点更新为当前节点的右子节点。
3. 重复步骤 2 和步骤 3，直到遍历结束。
[具体实现](https://github.com/Cc-Rank/Learning-Logs/blob/main/HandWriting/BinaryTreeTraversal.md/#前序morris遍历)

### [`#94` Morris 中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/)

> **Morris 遍历算法**是一种遍历二叉树的方法，它能将非递归的中序遍历空间复杂度降为 $O(1)$。

**Morris 遍历算法**整体步骤如下（假设当前遍历到的节点为 `node`）：

1. 如果 `node` 无左孩子，先将 `node` 的值加入答案数组，再访问 `node` 的右孩子，即 `node = node->right`。
2. 如果 `node` 有左孩子，则找到 `node` 左子树上最右的节点（即左子树中序遍历的最后一个节点， `node` 在中序遍历中的前驱结点），我们记为 `predecessor` 。根据 `predecessor` 的右孩子是否为空，进行如下操作。
   - 如果 `predecessor` 的右孩子为空，则将其右孩子指向 `node` ，然后访问 `node` 的左孩子，即 `node = node->left` 。
   - 如果 `predecessor` 的右孩子不为空，则此时其右孩子指向 `node`，说明我们已经遍历完 `node` 的左子树，我们将 `predecessor` 的右孩子置空，将 `node` 的值加入答案数组，然后访问 `node` 的右孩子，即 `node = node->right`。
3. 重复上述操作，直至访问完整棵树。
[具体实现](https://github.com/Cc-Rank/Learning-Logs/blob/main/HandWriting/BinaryTreeTraversal.md/#中序-morris-遍历)

### [`#145` Morris 后序遍历](https://leetcode.cn/problems/binary-tree-postorder-traversal/solution/er-cha-shu-de-hou-xu-bian-li-by-leetcode-solution/)

此外，Morris 后序遍历规则总结如下：

1. 如果当前节点的左子节点为空，则遍历当前节点的右子节点；
2. 如果当前节点的左子节点不为空，在当前节点的左子树中找到当前节点在中序遍历下的前驱节点：
   - 如果前驱节点的右子节点为空，将前驱节点的右子节点设置为当前节点，当前节点更新为当前节点的左子节点。
   - 如果前驱节点的右子节点为当前节点，将它的右子节点重新设为空。倒序输出从当前节点的左子节点到该前驱节点这条路径上的所有节点。当前节点更新为当前节点的右子节点。
3. 重复步骤 2 和步骤 3，直到遍历结束。

### 复杂度分析

- 时间复杂度：$O(n)$，其中 $n$ 是二叉树的节点数。没有左子树的节点只被访问一次，有左子树的节点被访问两次。
- 空间复杂度：$O(1)$。只操作已经存在的指针（树的空闲指针），因此只需要常数的额外空间。

## 2023.5.16

1) LeetCode 刷题：

    每日进度（5 / 5）

    中等题：`#1335.工作计划的最低难度（DP）`

    树形DP专项：
    - new：`#124.二叉树中的最大路径和`, `#2246.相邻字符不同的最长路径`, `#687.最长同值路径`, `#2538.最大价值和与最小价值和的差值`
    - 复习：`#543`

2) 复习 GAMES101 作业 1

## 2023.5.17

1) LeetCode 刷题：

    每日进度（5 / 5）

    简单题：`#2446`, `#283`

    中等题：`#1003.检查替换后的词是否有效（栈）`, `#970.强整数（枚举）`, `#1376.通知所有员工所需的时间（递归）`

2) 整理并完成 GAMES101 作业 1 的 [笔记](https://github.com/Cc-Rank/GAMES101/blob/main/Assignment/Assignment1/README.md)

## 2023.5.18

1) LeetCode 刷题：

    每日进度（6 / 5）

    简单题：`#485`

    中等题：`#1073.负二进制数相加`

    双指针专项：
    - new：`#209.长度最小的子数组`, `#713.乘积小于k的子数组`, `#1004.最大连续1的个数III`, `#1658.将x减到0的最少操作数`
    - 复习：`#3`, `#1234`

2) 复习 GAMES101 作业 2

## 2023.5.19

1) LeetCode 刷题：

    每日进度（7 / 5）

    中等题：`#1079.活字印刷（计数DP）`

    双指针专项：
    - new：`#15.三数之和`, `#16.最接近的三数之和`, `18.四数之和`, `#611.有效三角形的个数`, `#11.盛水最多的容器`, `#42.接雨水`
    - 复习：`#167`

    ~~TODO：Dijkstra(#407)~~

## 2023.5.20 - 2023.5.21

1) LeetCode 刷题：

    每日进度（2 / 10）

    简单题：`#LCP 33`

    困难题：`#1373.二叉搜索子树的最大键值和`

## 2023.5.22

1) LeetCode 刷题：

    每日进度（8 / 5）

    简单题：`#9`

    中等题：`#1080.根到叶路径上的不足节点`, [`#912.手写排序`](https://leetcode.cn/problems/sort-an-array/), `#380.O(1)时间插入、删除和获取随机元素（数组+哈希表）`

    困难题：`#882.细分图中的可到达节点（Dijkstra）`

    二分专项：
    - new：`#162.寻找峰值`, `#153.寻找旋转排序数组中的最小值`, `#33.搜索旋转排序数组`
    - 复习：`#34`, `#154`

## 2023.5.23

1) LeetCode 刷题：

    每日进度（5 / 5）

    简单题：`#242`, `#205`

    中等题：`#743.网络延迟时间（Dijkstra）`, `#1976.到达目的地的方案数（Dijkstra + DAG 上的 DP）`, `#1090.受标签影响的最大值（排序 + 哈希）`

### [`#743` Dijkstra 算法](https://leetcode.cn/problems/network-delay-time/)

> Dijkstra 算法由荷兰计算机科学家 E. W. Dijkstra 于 1956 年发现，1959 年公开发表。是一种求解 **非负权图** 上单源最短路径的算法。其主要思想是贪心。

将结点分成两个集合：已确定最短路长度的点集（记为 $S$ 集合）的和未确定最短路长度的点集（记为 $T$ 集合）。一开始所有的点都属于 $T$ 集合。

初始化 $dis(s) = 0$，其他点的 $dis$ 均为 $+\infty$。

然后重复这些操作：

1. 从 $T$ 集合中，选取一个最短路长度最小的结点，移到 $S$ 集合中。
2. 对那些刚刚被加入 $S$ 集合的结点的所有出边执行 **松弛** 操作。

直到 $T$ 集合为空，算法结束。

这里的 **松弛** 操作是指，用起点到节点 $x$ 的最短路长度加上从节点 $x$ 到节点 $y$ 的边的长度，去比较起点到节点 $y$ 的最短路长度，如果前者小于后者，就用前者更新后者。

> 可以这样理解，因为我们已经用了每一个「已确定节点」更新过了当前节点，无需再次更新（因为一个点不能多次到达）。而当前节点已经是所有「未确定节点」中与起点距离最短的点，不可能被其它「未确定节点」更新。所以当前节点可以被归类为「已确定节点」。

#### 实现

有多种方法来维护 1 操作中最短路长度最小的结点，不同的实现导致了 Dijkstra 算法时间复杂度上的差异。这里同时记录 $O(n^2)$ 的暴力做法实现和 $O(m \log m)$ 的优先队列做法实现。

- 暴力：不使用任何数据结构进行维护，每次 2 操作执行完毕后，直接在 $T$ 集合中暴力寻找最短路长度最小的结点。2 操作总时间复杂度为 $O(m)$，1 操作总时间复杂度为 $O(n^2)$，全过程的时间复杂度为 $O(n^2 + m) = O(n^2)$。

```cpp
vector<int> dijkstra(vector<vector<int>>& g, int start) {
    int n = g.size();
    vector<int> dist(n, inf);
    dist[start] = 0;
    vector<int> used(n);
    for (int i = 0; i < n; ++i) {
        int x = -1;
        for (int y = 0; y < n; ++y)
            if (!used[y] && (x == -1 || dist[y] < dist[x]))
                x = y;
        if (x < 0) break;
        used[x] = true;
        for (int y = 0; y < n; ++y)
            dist[y] = min(dist[y], dist[x] + g[x][y]);
    }
    return dist;
}
```

- 优先队列：和二叉堆类似，但使用优先队列时，如果同一个点的最短路被更新多次，因为先前更新时插入的元素不能被删除，也不能被修改，只能留在优先队列中，故优先队列内的元素个数是 $O(m)$ 的，时间复杂度为 $O(m \log{m})$。

```cpp
vector<int> dijkstra(vector<vector<pair<int, int>>>& g, int start) {
    vector<int> dist(g.size(), INT_MAX);
    dist[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;
    pq.emplace(0, start);
    while (!pq.empty()) {
        auto [d, x] = pq.top();
        pq.pop();
        if (d > dist[x]) continue;
        for (auto& [y, wt] : g[x]) {
            int new_d = dist[x] + wt;
            if (new_d < dist[y]) {
                dist[y] = new_d;
                pq.emplace(new_d, y);
            }
        }
    }
    return dist;
}
```

在稀疏图中，$m = O(n)$，使用优先队列实现的 Dijkstra 算法较 Bellman-Ford 算法具有较大的效率优势；而在稠密图中，$m = O(n^2)$，这时候使用暴力做法较二叉堆实现更优。

## 2023.5.24

1) LeetCode 刷题：

    每日进度（5 / 5）

    困难题：`#1377.T秒后青蛙的位置（DFS）`

    链表专项：
    - new：`#92.反转链表II`, `#25.K个一组翻转链表`, `#876.链表的中间结点`, `#143.重排链表`
    - 复习：`#206`, `#24`, `#141`, `#142`, `#234`

## 2023.5.25

1) LeetCode 刷题：

    每日进度（5 / 5）

    简单题：`#2451`

    中等题：`#647.回文子串`

    链表专项：
    - new：`#237.删除链表中的节点`, `#203.移除链表元素`, `#82.删除排序链表中的重复元素II`
    - 复习：`#19`, `#83`
