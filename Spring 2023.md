# 2023年春 学习logs

1）看完 [`LeetCode101`](https://github.com/changgyhub/leetcode_101)；

2）看完 `Algorithm`；

3）完成 `GAMES101` 的复习笔记；

4）初步展开实验；

## 2023.2.9

1) LeetCode 刷题：
`#435, #135, #455, #605, #1797, #452`

2) GAMES104 Lecture 5；

## 2023.2.10

1) LeetCode 刷题：
`#763, #167, #88, #142, #122, #76, #633, #680, #524`

### [`#142` 快慢指针](https://leetcode.cn/problems/linked-list-cycle-ii/)

> 为什么快慢指针中`ptr`与`slow`相遇的节点一定是入环点？
>
> 假设环外部分长度为 $a$ ，`slow`指针进入环后，又走了 $b$ 的距离与`fast`相遇。此时`fast`指针已经走完了环的n圈，因此`fast`走过的总距离为：$a + n(b + c) + b = a + (n + 1)b + n c$;
>
> 任意时刻，`fast`走过的距离都为`slow`指针的2倍，而且已知`slow`指针不可能绕环超过一圈，即相遇时，`slow`指针走过的距离为 $a + b$。因此有
>
> $$\begin{aligned} &a + (n + 1)b + nc \\ \Rightarrow &a = c + (n - 1)(b + c) \end{aligned}$$
>
>其中 $c$ 表示`slow`与`fast`指针首次相遇位置到入环点的距离，而 $(n - 1)(b + c)$ 则是 $n - 1$ 圈的环长。
>
>因此，只需要再添加一个`ptr`指针指向头结点，当它走完环外距离 $a$ 时，则会与在绕圈中的`slow`指针相遇，且相遇点恰好是入环点。

## 2023.2.11

1) LeetCode 刷题：
`#2335, #69, #34, #81, #154, #540`

2) GAMES104 Lecture 6；

### [`#69` 牛顿迭代法](https://leetcode.cn/problems/sqrtx/)

<div align="center">
 <img src="Figure/牛顿迭代法.png" width = "85%" >
</div>

> 我们选择 $x_0 = C$ 作为初始值。
>
>在每一步迭代中，我们通过当前的交点 $x_i$，找到函数图像上的点 $(x_i, x^2_i - C)$，作为一条斜率为 $f(x_i) = 2x_i$ 的直线，直线的方程为：
>$$\begin{aligned}y &= 2x_i(x - x_i) + x^2_i - C \\ &= 2x_ix - (x^2_i + C) \end{aligned}$$
>与横轴的交点为方程 $2x_ix - (x^2_i + C)$ 的解，即为新的迭代结果 $x_(x+1)$:
>$$x_{i+1} = \cfrac{1}{2}(x_i + \cfrac{C}{x_i})$$
>在进行 $k$ 次迭代后，$x_k$的值与真实的零点 $\sqrt{C}$ 足够接近，即可作为答案。

### 二分查找

> 二分查找通常分为 **左闭右开** 和 **左闭右闭** 两种写法.

左闭右开中，循环的区间为 $[left, right)$，所以有 `left = mid + 1` 和 `right = mid`。主循环判断条件为 $left < right$。

左闭右闭时，循环的区间为 $[left, right]$，所以有 `left = mid + 1` 和 `right = mid - 1`。主循环判断条件为 $left \leq right$。

在刷题时，应当思考如果最后区间只剩下少量的数，自己的写法是否会陷入死循环，如果某种写法无法跳出死循环，则考虑尝试另一种写法。

## 2023.2.12

1) LeetCode 刷题：
`#1138, #215, #347, #75, #451, #695, #547`

2) GAMES104 Lecture 7；

### DFS 和 BFS

用栈实现的深度优先搜索和用队列实现的广度优先搜索在写法上并没有太大差异，因此使用哪一种搜索方式需要根据实际的功能需求来判断。

## 2023.2.13

1) LeetCode 刷题：
`#1234, #417, #46, #77, #79, #257, #51, #934`

## 2023.2.14

1) LeetCode 刷题：
`#126, #130, #1124, #70, #198, #413, #64, #542`

## 2023.2.15

1) LeetCode 刷题：
`#1250, #221, #279, #91, #139, #300, #1, #1143, #967`

### [`#1250` 「裴蜀定理」](https://leetcode.cn/problems/check-if-it-is-a-good-array/)

> 对于不全为零的任意整数 $a$ 和 $b$，记 $g = gcd(a, b)$，其中 $gcd(a, b)$ 为  $a$ 和 $b$ 的最大公约数，则对于任意整数 $x$ 和 $y$ 都满足 $a \times x + b \times y$ 是 $g$ 的倍数，特别的，存在整数 $x$ 和 $y$ 满足 $a \times x + b \times y = g$。

### [`#279` 四平方和定理](https://leetcode.cn/problems/perfect-squares/)

> 四平方和定理证明了任意一个正整数都可以被表示为至多四个正整数的平方和。
>
> 同时四平方和定理包含了一个更强的结论：当且仅当 $n \neq 4^k \times (8m + 7)$ 时，$n$ 可以被表示为至多三个正整数的平方和。因此，当 $n = 4^k \times (8m + 7)$ 时,$n$ 只能被表示为四个正整数的平方和。此时我们可以直接返回 4。

当 $n \neq 4^k \times (8m + 7)$时，我们需要判断到底多少个完全平方数能够表示 $n$，我们知道答案只会是 $1, 2, 3$ 中的一个：

- 答案为1时，则必有 $n$ 为完全平方数；
- 答案为2时，则有 $n = a^2 + b^2$，我们只需要枚举所有的 $a (1 \leq a \leq \sqrt{n})$，判断 $n - a^2$ 是否为完全平方数即可；
- 答案为3时， 我们只需要检查答案为 $1$ 或 $2$ 的两种情况，即可利用排除法确定答案。

## 2023.2.16

1) LeetCode 刷题：
`#2341, #416, #474, #322, #72, #650, #10, #121`

## 2023.2.17

1) LeetCode 刷题：
`#1139, #123, #188`

2) algorithm 2-3 tree

## 2023.2.18

1) LeetCode 刷题：
`#1237, #309, #714, #213`

## 2023.2.19

NULL

## 2023.2.20

1) LeetCode 刷题：
`#2347, #53, #343, #583, #646, #376, #494, #241`

## 2023.2.21

1) LeetCode 刷题：
`#1326, #932, #312, #204, #504, #172, #415, #326, #39`

### 公倍数与公因数

> 利用 **辗转相除法** 可以很方便地求得两个数的最大公因数（greatest common divisor, gcd）；
> 将两个数相乘再除以最大公因数即可得到最小公倍数（least common multiple, lcm）。

```c
    int gcd(int a, int b) {
        return b == 0 ? a : gcd(b, a % b);
    }

    int lcm(int a, int b) {
        return a * b / gcd(a, b);
    }
```

> 进一步地，我们也可以通过扩展欧几里得算法（extend gcd）在求得 $a$ 和 $b$ 最大公因数的同时，也得到它们的系数 $x$ 和 $y$，从而使得 $ax + by = gcd(a, b)$，这组系数同样适用于[裴蜀定理](#1250-裴蜀定理)。

```cpp
    int xGCD(int a, int b, int& x, int& y) {
        if (!b) {
            x = 1, y = 2;
            return a;
        }
        int x1, y1;
        int gcd = xGCD(b, a % b, x1, y1);
        x = x1, y = x1 - (a / b) * y1;
        return gcd;
    }
```

### [`#204` 埃氏筛法](https://leetcode.cn/problems/count-primes/)

> 埃拉托斯特尼筛法（Sieve of Eratosthenes, 简称埃氏筛法）是非常常用的，判断一个整数是否是质数的方法。
> 其原理也十分易懂：从 $1$ 到 $n$ 遍历，假设当前遍历到 $m$ ,则把所有小于 $n$ 的，且是 $m$ 的倍数的整数标为合数；遍历完成后，没有被标为合数的数字即为质数。

这里还可以继续优化，对于一个质数 $x$，如果从 $2x$ 标记其实是冗余的，应该直接从 $x\times x$ 开始标记，因为 $2x, 3x, ...$ 这些数一定在 $x$ 之前就被其他数的倍数标记过了。

同时还有 **偶数一定不是质数**，**最小质因子一定小于等于开方数** 等优化方法。

## 2023.2.22

1) LeetCode 刷题：
`#1140, #384, #528, #382, #168, #67, #238, #169, #131`

### [`#384` Fisher-Yates 洗牌算法](https://leetcode.cn/problems/shuffle-an-array/)

> Fisher-Yates 洗牌算法，原理是通过随机交换位置来实现随机打乱，有正向和反向两种写法，且实现非常方便。时间复杂度为 $O(n)$;

具体的，实现算法如下：

- 设待原地打乱的数组 $nums$。
- 循环 $n$ 次，在第 $i$ 次循环中 $(0 \leq i \leq n)$：
  - 在 $[i, n)$ 中随机抽取一个下标 $j$；
  - 将第 $i$ 个元素与第 $j$ 个元素交换。

### [`#382` 水库算法](https://leetcode.cn/problems/linked-list-random-node/)

> 水库采样：遍历一次链表，在遍历到第 $m$ 个节点时，有 $\frac{1}{m}$ 的概率选择这个节点覆盖掉之前的节点选择。

简单证明。对于长度为n的链表的第m个节点，最后被采样的充要条件是它被选择，且之后的节点都没有被选择。这种情况发生的概率为 $\frac{1}{m} \times \frac{m}{m + 1} \times \frac{m + 1}{m + 2} \times \ldots \times \frac{n - 1}{n} = \frac{1}{n}$ 。因此每个点都有均等的概率被选择。

### [`#169` Boyer-Moore 投票算法](https://leetcode.cn/problems/majority-element/)

> Boyer-Moore Majority Vote 算法。如果我们把众数记为 $+1$，把其他数记为 $-1$，将它们全部加起来，显然和大于0，从结果本身我们可以看出众数比其他数多。

Boyer-Moore 算法的详细步骤：

- 我们维护一个候选众数 `candidate` 和它出现的次数 `count`。初始时 `candidate` 可以为任意值，`count` 为 `0`；
  
- 我们遍历数组 `nums` 中的所有元素，对于每个元素 `x`，在判断 `x` 之前，如果 `count` 的值为 `0`，我们先将 `x` 的值赋予 `candidate`，随后我们判断 `x`：
  - 如果 `x` 与 `candidate` 相等，那么计数器 `count` 的值增加 `1`；
  - 如果 `x` 与 `candidate` 不等，那么计数器 `count` 的值减少 `1`；
  
- 在遍历完成后，`candidate` 即为整个数组的众数。

sda #202

## 2023.2.23

1) LeetCode 刷题：
`#1238, #89, #202, #470, #462, #3, #461, #190, #136, #231, #342, #318, #338`

### [`#1238` 格雷编码](https://leetcode.cn/problems/circular-permutation-in-binary-representation/)

> 在一组数的编码中，若任意两个相邻的代码只有一位二进制数不同，则称这种编码为 **格雷码**（Gray Code），另外由于最大数与最小数之间也仅一位数不同，即 “首尾相连”，因此又称 **循环码** 或 **反射码**。

格雷码可以使用公式直接求出。第 $i$ $(i \geq 0)$ 个格雷码即为：
$$g_i=i \oplus \lfloor \frac{i}{2} \rfloor$$
其中 $\oplus$ 表示按位异或运算。特别的，如果该组编码中第一个整数不为 $0$ ，则需要将求出的结果的每一项都与第一个整数 $start$ 按位进行异或运算：
$$ g_i=i \oplus \lfloor \frac{i}{2} \rfloor \oplus start$$

### [`#461` Brian Kernighan 算法](https://leetcode.cn/problems/hamming-distance/)

> 具体的，该算法可以被描述为这样一个结论：记 $f(x)$ 表示 $x$ 和 $x-1$ 进行运算所得的结果,即 $f(x) = x$ & $(x-1)$ ，那么 $f(x)$ 恰为 $x$ 删去其二进制表示中最右侧的 $1$ 的结果。

例如对于二进制表示 `11110100`，减去 `1` 得到 `11110011`，这两个数按位与得到 `11110000` 。

此外，$n$ & $(-n)$ 可以得到 $n$ 的位级表示中最低的那一位，例如对于二进制表示 `11110100`，取负得到 `00001100`，这两个数按位与得到 `00000100` 。

## 2023.2.24

1) LeetCode 刷题：
`#2357, #268, #693, #476, #260, #137, #448, #48, #240, #74, #769, #232`

## 2023.2.25

1) LeetCode 刷题：
`#1247, #2, #155, #20`

## 2023.2.26

1) LeetCode 刷题：
`#1255, #739, #4`

2) LeetCode 周赛：
`#6369, #6368, #6367(#2574, #2575, #2576)`

## 2023.2.27

1) LeetCode 刷题：
`#1144, #5, #768, #21, #23, #218, #1792, #239`

### [`#23` 优先队列](https://leetcode.cn/problems/merge-k-sorted-lists/)

> **优先队列**（priority queue）可以在 $O(1)$ 时间内获得最大值，并且可以在 $O(\log n)$ 时间内取出最大值或插入任意值。其常常用 **堆**（heap）来实现。堆是一个完全二叉树，其每个节点的值总是大于等于子节点的值。

实际实现堆时，我们通常用一个长度为 $N + 1$ 的数组而不是指针建立一个树。这是因为堆是完全二叉树，所以用数组表示时，位置 $i$ 的节点的父节点位置一定为 $i / 2$，而它的两个子节点的位置又一定分别为 $2i$ 和 $2i + 1$。要注意的是，方便起见，我们通常不会使用下标为 $0$ 的位置。

```cpp
vector<int> heap;

int top() {
    return heap[1];
}

void push(int k) {
    heap.push_back(k);
    swim(heap.size() - 1);
}

void pop() {
    heap[1] = heap.back();
    heap.pop_back();
    sink(1);
}

// 上浮
void swim(int pos) {
    while (pos > 1 && heap[pos / 2] < heap[pos]) {
        swap(heap[pos / 2], heap[pos]);
        pos >>= 1;
    }
}

// 下沉
void sink(int pos, int N) {
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

## 2023.2.28

1) LeetCode 刷题：
`#2363, #128, #141, #149, #47, #332, #303, #304, #560, #225`

1) Algorithm Chapter 2；

### [`#332` 欧拉路径（Hierholzer 算法）](https://leetcode.cn/problems/reconstruct-itinerary/)

> 给定一个 $n$ 个点 $m$ 条边的图，要求从指定的顶点出发，经过所有的边恰好一次。

这种 $\lceil$ 一笔画 $\rfloor$ 问题与欧拉图或者半欧拉图有着紧密的联系，其定义如下：

- 通过图中所有边恰好一次且行遍所有顶点的通路称为欧拉通路；
- 通过图中所有边恰好一次且行遍所有顶点的回路称为欧拉回路；
- 具有欧拉回路的无向图称为欧拉图；
- 具有欧拉通路但不具有欧拉回路的无向图称为半欧拉图。

> ***Hierholzer 算法*** 用于在连通图中寻找欧拉路径，其流程如下：
>
> 1. 从起点出发，进行深度优先搜索。
> 2. 每次沿着某条边从某个顶点移动到另外一个顶点的时候，都需要删除这条边。
> 3. 如果没有可移动的路径，则将所在节点加入到栈中，并返回。

## 2023.3.1

1) LeetCode 刷题：
`#2373, #503, #217, #697, #594, #674, #287, #313, #870`

## 2023.3.2

1) LeetCode 刷题：
`#面试题 05.02, #307`

### [`#307` 线段树、树状数组](https://leetcode.cn/problems/range-sum-query-mutable/)

#### 线段树

> 线段树 $segmentTree$ 是一个二叉树，每个结点保存数组 $nums$ 在区间 $[s, e]$ 的最小值、最大值或者总和等信息。线段树可以用树也可以用数组（堆式存储）来实现。对于数组实现，假设根节点的下标为 $0$，如果一个结点在数组的下标为 $node$，那么它的左子结点下标为 $node \times 2 + 1$，右子结点下标为 $node \times 2 + 2$。

- 建树 $build$ 函数

  我们在结点 $node$ 保存数组 $nums$ 在区间 $[s, e]$ 的总和。
  - $s = e$ 时，结点 $node$ 是叶子结点，它保存的值等于 $nums[s]$。
  - $s < e$ 时，结点 $node$ 是左子结点保存区间 $[s, \lfloor \frac{s + e}{2} \rfloor]$ 的总和,右子结点保存区间 $[\lfloor \frac{s + e}{2} \rfloor + 1, e]$ 的总和，那么结点 $node$ 保存的值等于它的两个子结点保存的值之和。

  假设 $nums$ 的大小为 $n$，我们规定根节点 $node = 0$ 保存区间 $[0, n - 1]$ 的总和，然后自下而上递归地建树。

- 单点修改 $change$ 函数
  
  当我们要修改 $nums[index]$ 的值时，我们找到对应区间 $[index, index]$ 的叶子结点，直接修改叶子结点的值为 $val$，并自下而上递归地更新父结点的值。

- 范围求和 $range$ 函数

  给定区间 $[left, right]$ 时，我们将区间 $[left, right]$ 拆成多个结点对应的区间。

  - 如果结点 $node$ 对应的区间与 $[left, right]$ 相同，可以直接返回该结点的值，即当前区间的和。
  - 如果结点 $node$ 对应的区间与 $[left, right]$ 不同，设左子结点对应的区间右端点为 $m$ 那么将区间 $[left, right]$ 沿点 $m$ 拆成两个区间，分别计算左子结点和右子结点。

  我们从根节点开始递归地拆分区间 $[left, right]$。

#### [树状数组](https://blog.csdn.net/qq_40941722/article/details/104406126)

> 树状数组是一种可以动态维护序列前缀和的数据结构（序列下标从 $1$ 开始），他的功能是：
>
> - 单点修改 $add(index, val)$：把序列第 $index$ 个数增加 $val$；
> - 区间查询 $prefixSum(index)$：查询前 $index$ 个元素的前缀和。

因为题目要求实现更新 $nums$ 在某个位置的值，因此我们保存原始的 $nums$ 数组。

- 构造函数
  
  树状数组初始对应一个零序列，因此我们遍历 $nums$ 数组，调用 $add$ 函数来更新树状数组。

- $update$ 函数

  获取 $nums$ 在 $index$ 的增加值，调用 $add$ 函数更新树状数组，并更新 $nums[index] = val$。
- $sumRange$ 函数
  
  区间和 $[left, right]$ 可以转化为两个前缀和之差，调用树状数组的 $prefixSum$ 函数获取前 $right + 1$ 元素的前缀和 $sum1$ 和前 $left$ 个元素的前缀和 $sum2$, 返回 $sum1 - sum2$。

## 2023.3.3

1) LeetCode 刷题：
`#1487, #206, #24, #160, #234, #83, #328, #19`

## 2023.3.4

1) LeetCode 刷题：
`#982, #148, #147, #104, #110`

2) LeetCode 双周赛：
~~`#6312, #6311, #6313`~~`#2578, #2579, #2580`

## 2023.3.5

1) LeetCode 刷题：
`#1599`

2) LeetCode 周赛：
~~`#6307, #6308`~~`#2582, #2583`

## 2023.3.6

1) LeetCode 刷题：
`#1653, #52, #1129`

## 2023.3.7

1) LeetCode 刷题：
`#1096, #55`

## 2023.3.8

1) LeetCode 刷题：
`#剑指Offer 47, #2585, #17, #78, #748, #2397, #剑指Offer 58, #1601, #216`

1) GAMES104 Lecture 8；

## 2023.3.9

1) LeetCode 刷题：
`#2379, #22, #2584, #543, #437, #112, #113, #101, #1110, #637, #144, #105`

## 2023.3.10

1) LeetCode 刷题：
`#1590`
2) 报名《华为软件精英挑战赛》并阅读相关文档
3) 给最近的工作建立了Github repository

### [`#1590` 同余](https://leetcode.cn/problems/make-sum-divisible-by-p/)

> 两个数 $x$ 和 $y$，如果 $(x - y) \mod{p} = 0$，则称 $x$ 与 $y$ 对模 $p$ 同余，记作
> $$x \equiv y ( \mod{p})$$
>例如 $42 \equiv 12 ( \mod 10)$，$-17 \equiv 3( \mod 10)$。

## 2023.3.11

1) LeetCode 刷题：
`#面试题 17.05 字母与数字`

## 2023.3.12

1) LeetCode 刷题：
`#1617, #295`

## 2023.3.13

1) LeetCode 刷题：
`#2383`

## 2023.3.27

1) 华为软挑初赛结束，两个礼拜练习赛，三天正式赛
2) 休息一天。

## 2023.3.28

1) LeetCode 刷题：
`#1092`

2) GAMES401  Lecture 1;

    配置好了基本环境

## 2023.3.29

1) LeetCode 刷题：
`#1641`

## 2023.3.30

1) LeetCode 刷题：
`#1637`

## 2023.3.31

1) LeetCode 刷题：
`#2367, #99, #669`

### [`#99` 二叉查找树](https://leetcode.cn/problems/recover-binary-search-tree/)

> **二叉查找树**（Binary Search Tree, BST）是一种特殊的二叉树：对于每个父节点，其左子树中所有节点的值小于等于父节点的值，其右子树中所有节点的值大于等于父节点的值。因此对于一个二叉查找树，我们可以在 $O(\log{n})$ 的时间内查找一个值是否存在。

一个二叉查找树的实现如下：

```cpp
template <class T>
class BST {
    struct Node {
        T data;
        Node* left;
        Node* right;
    };

    Node* root;

    Node* makeEmpty(Node* t) 
    {
        if (t == nullptr) return nullptr;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        return nullptr;
    }

    Node* insert(Node* t, T x)
    {
        if (t == nullptr)
        {
            t = new Node;
            t->data = x;
            t->left = t->right = nullptr;
        }
        else if (x < t->data)
        {
            t->left = insert(t->left, x);
        }
        else if (x > t->data)
        {
            t->right = insert(t->right, x);
        }
        return t;
    }

    Node* find(Node* t, T x)
    {
        if (t == nullptr)
            return nullptr;
        if (x < t->data)
            return find(t->left, x);
        if (x > t->data)
            return find(t->right, x);
        return t;
    }

    Node* findMin(Node* t)
    {
        if (t == nullptr || t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    Node* findMax(Node* t)
    {
        if (t == nullptr || t->right == nullptr)
            return t;
        return findMax(t->right);
    }

    Node* remove(Node* t, T x)
    {
        Node* temp;
        if (t == nullptr)
            return nullptr;
        else if (x < t->data)
            t->left = remove(t->left, x);
        else if (x > t->data)
            t->right = remove(t->right, x);
        else if (t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->right, t->data);
        }
        else
        {
            temp = t;
            if (t->left == nullptr)
                t = t->right;
            else if (t->riht == nullptr)
                t = t->left;
            delete temp;
        }
        return t;
    }
public:
    BST(): root(nullptr) {}
    
    ~BST() 
    {
        root = makeEmpty(root);
    }

    void insert(T x)
    {
        root = insert(root, x);
    }

    void remove(T x)
    {
        root = remove(root, x);
    }
};
```

## 2023.4.1

1) LeetCode 刷题：
`#831`

### [`#831` 正则表达式](https://leetcode.cn/problems/masking-personal-information/)

> TODO:
>
> 需要康一康C++怎么运用`正则表达式`。

## 2023.4.2

1) LeetCode 刷题：
`#1039`

## 2023.4.3

1) LeetCode 刷题：
`#208, #1053, #1032, #226, #617, #572, #404, #513, #538, #235, #530, #236`

### [`#208` 前缀树](https://leetcode.cn/problems/implement-trie-prefix-tree/)

> **字典树**（Trie）用于判断字符串是否存在或者是否具有某种字符串前缀。

### [`#572` 另一棵树的子树](https://leetcode.cn/problems/subtree-of-another-tree/)

> TODO:
>
> 需要再回顾一下`kmp`解法和`树哈希`解法。

## 2023.4.4

1) LeetCode 刷题：
`#94, #1000, #889, #106, #145, #109, #897, #653`

### [`#94` Morris 中序遍历](https://leetcode.cn/problems/binary-tree-inorder-traversal/)

> **Morris 遍历算法**是一种遍历二叉树的方法，它能将非递归的中序遍历空间复杂度降为 $O(1)$。

**Morris 遍历算法**整体步骤如下（假设当前遍历到的节点为 $x$）：

1. 如果 $x$ 无左孩子，先将 $x$ 的值加入答案数组，再访问 $x$ 的右孩子，即 $x=x.right$ 。
2. 如果 $x$ 有左孩子，则找到 $x$ 左子树上最右的节点（即左子树中序遍历的最后一个节点， $x$ 在中序遍历中的前驱结点），我们记为 $predecessor$ 。根据 $predecessor$ 的右孩子是否为空，进行如下操作。

   - 如果 $predecessor$ 的右孩子为空，则将其右孩子指向 $x$ ，然后访问 $x$ 的左孩子，即 $x=x.left$ 。
   - 如果 $predecessor$ 的右孩子不为空，则此时其右孩子指向 $x$，说明我们已经遍历完 $x$ 的左子树，我们将 $predecessor$ 的右孩子置空，将 $x$ 的值加入答案数组，然后访问 $x$ 的右孩子，即 $x=x.right$ 。
3. 重复上述操作，直至访问完整棵树。

## 2023.4.5

1) LeetCode 刷题：
`#450, #2427, #337`

## 2023.4.6

1) LeetCode 刷题：
`#1017`

2) 复习动态规划 - 背包 DP
   - [0-1 背包](https://www.luogu.com.cn/problem/P2871)
   - [完全背包](https://www.luogu.com.cn/problem/P1616)
   - [多重背包](https://leetcode.cn/problems/number-of-ways-to-earn-points/)

## 2023.4.7

1) LeetCode 刷题：
`#1040`

## 2023.4.8

1) LeetCode 刷题：
`#1125`

> TODO:
>
> 在学习状压 DP 时可以再回头看看 [#1125](https://leetcode.cn/problems/smallest-sufficient-team/)。
