---
title: 奇技淫巧
date: 2018-11-02
---

很多遇到的问题、技巧、奇奇怪怪的数学性质、结论等都记录在这里。

<!-- more -->

<!-- toc -->

# Coding 相关

- 时常遭受 Runtime Error？或者时常会写一些奇奇怪怪的小错误？来让编译器帮你看看：
    + [Catching silly mistakes with GCC - Codeforces](https://codeforces.com/blog/entry/15547)
    + [Program Instrumentation Options - GCC](https://gcc.gnu.org/onlinedocs/gcc/Instrumentation-Options.html)
    + `-Wall -Wextra -Wshadow -Wconversion -Wfloat-equal -Wlogical-op -Wformat=2 -Wshift-overflow=2 -pedantic -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -D_FORTIFY_SOURCE=2 -fsanitize=address -fsanitize=undefined -fno-sanitize-recover=all`
    + `-fno-sanitize-recover=all` 用来保证对拍的时候，遇到 sanity 问题时会之间带着 error code 结束。
    + 可以把这些写成 bashrc 的一个函数：
```bash
ccc() {
    g++ -std=c++17 -Wall -Wextra -Wshadow -Wconversion \
        -Wfloat-equal -Wlogical-op -Wformat=2 -Wshift-overflow=2 \
        -pedantic -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC \
        -D_FORTIFY_SOURCE=2 -fsanitize=address \
        -fsanitize=undefined -fno-sanitize-recover=all \
        -g "$@" -o ${1/\.cpp/}
}
```


# 数论 (or 数字)

## GCD
- `前缀GCD` 只有 \\(\mathcal{O}(\log{a})\\) 种取值，并单调不严格递减，呈块状
    + [HDU 5726](http://acm.hdu.edu.cn/showproblem.php?pid=5726)

## 二进制运算
- `前缀AND` 只有 \\(\mathcal{O}(\log{a})\\) 种取值（考虑每一位的情况），并单调不增，呈块状，值小于或等于 `前缀最小值`
  `前缀OR` 只有 \\(\mathcal{O}(\log{a})\\) 种取值，并单调不减，呈块状，值大于或等于 `前缀最大值`
    + {% post_link bcpc-2017-I %}


# 图论

## 树
- 树的邻接矩阵（对称阵、无自环）的秩等于树的最大匹配数
    + [Codeforce 1067E](https://codeforces.com/contest/1067/problem/E)

## 二分图
- 二分图的最大匹配数等于最小点覆盖数（可构造），二分图的最大独立集可由点集与最小点覆盖的差集构造
    + {% post_link bcpc-2017-pre-E %}

-  二分图的题目除了可以藏在树、网格图中以外，某一位异或为一时才有边、和为大于 2 的质数等很多奇奇怪怪的无奇环的性质都可能是。

## 网络流
- 平面图最小割（最大流）等价于对偶图中的最短路
    + [BZOJ 1001](https://www.lydsy.com/JudgeOnline/problem.php?id=1001)

## 无向图
- 走重复点、边的回路可抽取为若干个简单环
    + 按顺序处理点，重复出现点就可以把这一段简单环单独抽出来，重复这样抽取就可以把回路抽取为若干个简单环
