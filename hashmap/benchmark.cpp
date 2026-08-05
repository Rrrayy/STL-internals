  #include "R_hashmap.hpp"
  #include <unordered_map>
  #include <chrono>
  #include <iostream>
  #include <random>

  void test_benchmark() {
        const int N = 1000000;
        std::mt19937 gen(42);
        std::uniform_int_distribution<int> dist(0, 100000000);

        std::cout << " 100万键值对 插入+查找 对比"<<std::endl;

        R_hashmap<int, int> mine;
        auto t1 = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < N; i++) mine.insert(dist(gen), i);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "my_hashmap 插入: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";

        std::unordered_map<int, int> stdm;
        t1 = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < N; i++) stdm.insert({dist(gen), i});
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "std 插入: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms\n";
  }

  int main() {
        test_benchmark();
        return 0;
  }
