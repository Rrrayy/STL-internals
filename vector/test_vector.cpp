#include"R_vector.hpp"
#include<vector>
#include<chrono>
#include<iostream>

void test_speed(){
    const int N = 1000000;
    R_vector<int> r_vec;
    auto t1 = std::chrono::high_resolution_clock::now(); 
    for(int i=0 ;i<N; ++i)  r_vec.push_back(i);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto r_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    std::vector<int> std_vec;
    t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; i++) std_vec.push_back(i);
    t2 = std::chrono::high_resolution_clock::now();
    auto std_ms = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    
    std::cout << "R_vector:   " << r_ms << " ms"<<std::endl;
    std::cout << "std::vector: " << std_ms << " ms"<<std::endl;

    // r_vec 略慢正常：std 用 allocator 分离分配/构造 + 内存对齐优化
}

void test_grow(){
    for(double factor : {1.5 , 2.0}){
        size_t cap =0 ,total=0 ,grows=0;
        for(size_t n =1 ; n<=1000000; ++n){
            if(n>cap){
                cap = cap ? (size_t)(cap * factor) : 1;
                if(cap < n) cap = n;
                total += cap;
                grows++;
            }
        }
        std::cout << "factor=" << factor << ": 扩容 " << grows << " 次, 累计分配 " << total << " 格, 最终容量 " << cap << "\n";    
    }
}

void test_data(){
    R_vector<int> v;
    for(int i = 0; i < 5; i++) v.push_back(i * 10);
    bool ok = (v.data() == &v[0]);
    std::cout << "data() == &v[0] ? " << (ok ? "true" : "false") << "\n";
    int* p = v.data();
    for(int i = 0; i < 5; i++) std::cout << p[i] << " ";
}

int main(){
        test_speed();
        test_grow();
        test_data();
        return 0;


}
