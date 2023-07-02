# 预处理质数的板子

这是灵神的板子，这里记录一下。

## 埃氏筛

```cpp
const int MX = 1e6;
vector<int> primes;

int init = []() {
    bool np[MX + 1]{};
    for (int i = 2; i <= MX; ++i)
        if (!np[i]) {
            primes.push_back(i);
            for (int j = i; j <= MX / i; ++j) // 避免溢出的写法
                np[i * j] = true;
        }
    primes.push_back(MX + 1);
    primes.push_back(MX + 1); // 保证下面下标不会越界
    return 0;
}();
```

## 线性筛

```cpp
const int MX = 1e6;
vector<int> primes;

int init = []() {
    bool np[MX + 1]{};
    for (int i = 2; i <= MX; ++i) {
        if (!np[i]) primes.push_back(i);
        for (int p: primes) {
            if (i * p > MX) break;
            np[i * p] = true;
            if (i % p == 0) break;
        }
    }
    primes.push_back(MX + 1);
    primes.push_back(MX + 1); // 保证下面下标不会越界
    return 0;
}();
```