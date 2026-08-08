# STL Internals

从零手写 C++ 标准库核心容器，以源码级实现剖析 STL 底层机制：内存管理、迭代器语义与复杂度保证。

## 实现

| 容器 | 文件 | 核心机制 |
|------|------|----------|
| vector | `vector/R_vector.hpp` | 三指针布局 · 1.5 倍扩容 |
| unordered_map | `hashmap/R_hashmap.hpp` | 拉链法 · 负载因子 0.75 · rehash |
| list | `list/R_list.hpp` | 哨兵节点环 · splice O(1) |
| string | `string/R_string.hpp` | SSO（≤15B 栈上内联）|
| forward_list | `forward_list/R_forward_list.hpp` | 单向链表 · insert_after/erase_after |
| deque | `deque/R_deque.hpp` | 中控器 + buffer · 两级索引 · 四指针迭代器 |
| priority_queue | `priority_queue/R_priority_queue.hpp` | heap 三算法 · 比较器可插拔 |

## 目录结构

```
STL-internals/
├── vector/          R_vector
├── hashmap/         R_hashmap
├── list/            R_list
├── string/          R_string
├── forward_list/    R_forward_list
├── deque/           R_deque
└── priority_queue/  R_priority_queue
```

## 构建与测试

各目录独立构建，C++17，兼容 GCC / Clang：

```bash
cd deque
g++ -std=c++17 -Wall -Wextra test_deque.cpp -o test && ./test
g++ -std=c++17 -fsanitize=address -g test_deque.cpp -o test_asan && ./test_asan
```

## 质量

- `-Wall -Wextra` 双编译器零警告
- AddressSanitizer 全量验证零泄漏
- 复杂度对齐标准：size/splice/insert/erase O(1)，push/pop O(log n)
- priority_queue 实测较 std 快约 22%（Release，100 万次）

## 演进

- [x] 七容器（vector/hashmap/list/string/forward_list/deque/priority_queue）
- [ ] 迭代器五类别 · 失效速查
- [ ] SGI 两级分配器
