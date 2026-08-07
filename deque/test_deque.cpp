#include "R_deque.hpp"
#include <cassert>
#include <utility>

void test_deque(){
	R_deque<int> d;
	assert(d.empty() && d.size() == 0);

	for(int i = 1; i <= 6; ++i) d.push_back(i);
	assert(d.size() == 6);

	for(int i = 0; i < 6; ++i) assert(d[i] == i + 1);

	int expect = 1;
	for(auto it = d.begin(); it != d.end(); ++it)
		assert(*it == expect++);

	expect = 6;
	auto it = d.end();
	while(it != d.begin()){
		--it;
		assert(*it == expect--);
	}

	d.push_front(0);
	d.push_front(-1);
	assert(d[0] == -1 && d[1] == 0 && d[2] == 1);
	assert(d.size() == 8);

	for(int i = 0; i < 8; ++i) assert(d[i] == i - 1);

	
	d.pop_back();                             
	d.pop_front();                            
	assert(d.size() == 6 && d[0] == 0 && d[5] == 5);

	
	while(!d.empty()) d.pop_back();
	assert(d.empty());

	
	for(int i = 0; i < 10; ++i) d.push_back(i * 10);
	assert(d.size() == 10 && d[9] == 90);

	
	R_deque<int> d2(d);                       
	assert(d2.size() == 10 && d2[3] == 30);
	R_deque<int> d3(std::move(d));            
	assert(d3.size() == 10 && d3[9] == 90 && d.empty());

	
	R_deque<int> e;
	assert(e.begin() == e.end());
}

int main(){
	test_deque();
	return 0;
}