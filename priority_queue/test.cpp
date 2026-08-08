#include "R_priority_queue.hpp"
#include <iostream>
#include <queue>
#include <ctime>
#include <cassert>
#include <functional>          // greater

void test_functional(){
	R_priority_queue<int> pq;
	pq.push(3); pq.push(1); pq.push(5); pq.push(2); pq.push(4);
	assert(pq.size() == 5 && !pq.empty());

	int expect = 5;                      
	while(!pq.empty()){
		assert(pq.top() == expect);     // 5,4,3,2,1
        expect--;
		pq.pop();
	}
	assert(pq.empty());
	std::cout << "功能测试通过大顶堆：乱序 push从大到小 pop" << std::endl;
}

void test_min_heap(){
	R_priority_queue<int, std::greater<int>> pq;   // 小顶堆
	pq.push(3); pq.push(1); pq.push(4); pq.push(2);
	int expect = 1;                       
	while(!pq.empty()){
		assert(pq.top() == expect++);     // 1,2,3,5
		pq.pop();
	}
	std::cout << "小顶堆测试通过greater<int>  从小到大 pop" << std::endl;
}

struct Task{
	int priority;
	int id;
};

struct CmpByPriority{                     // 比较器：priority 大的优先
	bool operator()(const Task& a, const Task& b) const{
		return a.priority < b.priority;   
	}
};

void test_custom_type(){
	R_priority_queue<Task, CmpByPriority> pq;
	pq.emplace(Task{3, 1});               // emplace：原地构造
	pq.emplace(Task{5, 2});
	pq.emplace(Task{4, 3});

	assert(pq.top().id == 2);             // priority 5 的先出
	pq.pop();
	assert(pq.top().id == 3);             // priority 4
	pq.pop();
	assert(pq.top().id == 1);             // priority 3
	std::cout << "自定义类型+比较器测试通过" << std::endl;
}

void test_make_heap(){
	std::vector<int> v = {4, 1, 7, 3, 9, 2, 8, 5, 6};
	make_heap(v, std::less<int>());
	assert(v.front() == 9);              

	std::vector<int> v2 = {4, 1, 7, 3, 9, 2, 8, 5, 6};
	make_heap(v2, std::less<int>());
	for(int n = (int)v2.size(); n > 1; --n){
		std::swap(v2[0], v2[n-1]);
		int i = 0;
		while(true){
			int l = 2*i+1, r = 2*i+2, big = i;
			if(l < n-1 && v2[big] < v2[l]) big = l;
			if(r < n-1 && v2[big] < v2[r]) big = r;
			if(big == i) break;
			std::swap(v2[i], v2[big]);
			i = big;
		}
	}
	for(size_t i = 1; i < v2.size(); ++i)
		assert(v2[i-1] <= v2[i]);         // 升序
	std::cout << "make_heap 测试通过" << std::endl;
}

void test_swap(){
	R_priority_queue<int> a, b;
	a.push(1); a.push(2);
	b.push(9); b.push(8);
	a.swap(b);
	assert(a.top() == 9 && b.top() == 2); 
	std::cout << "swap 测试通过 " << std::endl;
}
 
void benchmark_pq(size_t ntimes){
	std::vector<long long> result;
	size_t begin = clock();
	R_priority_queue<int> pq;
	for(size_t i = 0; i < ntimes; ++i){
		pq.push((int)i % 1000);
		if(i % 10 == 0) { result.push_back(pq.top()); pq.pop(); }
	}
	while(!pq.empty()){ result.push_back(pq.top()); pq.pop(); }
	size_t cost = clock() - begin;
	std::cout << "R_priority_queue " << ntimes << " 次: " << cost << " ms（结果数 " << result.size() << "）" << std::endl;
}

void benchmark_std(size_t ntimes){
	std::vector<long long> result;
	size_t begin = clock();
	std::priority_queue<int> pq;
	for(size_t i = 0; i < ntimes; ++i){
		pq.push((int)i % 1000);
		if(i % 10 == 0) { result.push_back(pq.top()); pq.pop(); }
	}
	while(!pq.empty()){ result.push_back(pq.top()); pq.pop(); }
	size_t cost = clock() - begin;
	std::cout << "std::priority_queue " << ntimes << " 次: " << cost << " ms（结果数 " << result.size() << "）" << std::endl;
}

int main(){
	test_functional();
	test_min_heap();
	test_custom_type();
	test_make_heap();
	test_swap();
	benchmark_pq(1000000);
	benchmark_std(1000000);
	return 0;
}