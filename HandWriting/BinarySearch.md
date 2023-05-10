# [`#35` 二分查找](https://leetcode.cn/problems/search-insert-position/)

## 算法

二分查找也常被成为二分法或者折半查找，每次查找时通过将待查找区间分成两部分并只取一部分继续查找，将查找的复杂度大大减少。对于一个长度为 $O(n)$ 的数组，二分查找的时间复杂度为 $O(\log{n})$。

## 实现

这里的实现以左闭右开为例。其中，循环的区间为 $[left, right)$，所以有 `left = mid + 1` 和 `right = mid`。主循环判断条件为 $left < right$。

### std::lower_bound

```cpp
template< class ForwardIt, class T, class Compare >
ForwardIt lower_bound( ForwardIt first, ForwardIt last, const T& value, Compare comp );
```

Returns an iterator pointing to the first element in the range [`first`, `last`) that does not satisfy `element < value` (or `comp(element, value)`), (i.e. greater or equal to), or `last` if no such element is found.

### 简单实现lower_bound

```cpp
int lower_bound(vector<int>& nums, int target) {
    int l = 0, r = nums.size(), mid;
    while (l < r) {
        mid = l + ((r - l) >> 1);
        if (nums[mid] >= target)
            r = mid;
        else
            l = mid + 1;
    }
    return l;;
}
```

### std::upper_bound

```cpp
template< class ForwardIt, class T, class Compare >
ForwardIt upper_bound( ForwardIt first, ForwardIt last, const T& value, Compare comp );
```

Returns an iterator pointing to the first element in the range [`first`, `last`) such that `element < value` (or `comp(element, value)`) is `true`, (i.e. strictly greater), or `last` if no such element is found.

### 简单实现upper_bound

```cpp
int upper_bound(vector<int>& nums, int target) {
    int l = 0, r = nums.size(), mid;
    while (l < r) {
        mid = l + ((r - l) >> 1);
        if (nums[mid] > target)
            r = mid;
        else
            l = mid + 1;
    }
    return l;;
}
```
