#include "R_hashmap.hpp"
#include <iostream>
#include <random>

void test_basic() {
    R_hashmap<int, int> m;
    for(int i = 0; i < 100000; i++) m.insert(i, i * 2);   
    bool all_hit = true;
    int v;
    for(int i = 0; i < 100000; i++)                       
            if(!m.find(i, v) || v != i * 2) all_hit = false;   
    std::cout << "插入10万+全命中: " << (all_hit ? "t" : "f") << "\n";

    m.insert(42, 999);                        
    m.find(42, v);
    std::cout << "重复key更新: v=" << v  << (v == 999 ? "true" : "false") <<std::endl;

    for(int i = 0; i < 50000; i++) m.erase(i);             
    std::cout << "erase一半后 size=" << m.size() << (m.size() == 50000 ? "true" : "false") <<std::endl;
  }

void test_rehash() {
    R_hashmap<int, int> m;
    size_t prev = m.bucket_count();
    for(int i = 1; i <= 100; i++) {
        m.insert(i, i);
        if(m.bucket_count() != prev) {                      // 桶数变了 = rehash 发生
            std::cout << "插入第 " << i << " 个元素后: 桶数 " << prev<< " → " << m.bucket_count()<< "  (触发点: " << i << " > " << (size_t)(prev * 0.75) << ")"<<std::endl;
            prev = m.bucket_count();
            }
        }
  }

void test_distribution() {
    R_hashmap<int, int> m;
    std::mt19937 gen(42);                                
    std::uniform_int_distribution<int> dist(0, 100000000);
    for(int i = 0; i < 12000; i++) m.insert(dist(gen), i);

    size_t c0 = 0, c1 = 0, c2 = 0, c3 = 0, c4 = 0;         // 链长 0/1/2/3/≥4 的桶数
    for(size_t len : m.chain_len()) {
            if(len == 0) c0++;
            else if(len == 1) c1++;
            else if(len == 2) c2++;
            else if(len == 3) c3++;
            else c4++;
        }
    size_t total = m.bucket_count();
    std::cout << "桶数=" << total << " 负载=" << (float)m.size() / total << "\n";
        printf("链长0: %.1f%%  (理论 47.2%%)\n", 100.0 * c0 / total);
        printf("链长1: %.1f%%  (理论 35.4%%)\n", 100.0 * c1 / total);
        printf("链长2: %.1f%%  (理论 13.3%%)\n", 100.0 * c2 / total);
        printf("链长3: %.1f%%  (理论  3.3%%)\n", 100.0 * c3 / total);
        printf("链长≥4: %.1f%% (理论 ~0.8%%)\n", 100.0 * c4 / total);
        std::cout << "（实测 vs 理论误差应在 ±3% 内——泊松分布眼见为实）\n\n";
  }

  int main() {
        test_basic();
        test_rehash();
        test_distribution();
        return 0;
  }
