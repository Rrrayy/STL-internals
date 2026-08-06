#include"R_list.hpp"
#include <iostream>
#include<cassert>
#include<utility>

void test_list(){            
    R_list<int> l;
	assert(l.empty() && l.size() == 0);
	l.push_back(1); l.push_back(2); l.push_front(0);
	assert(l.size() == 3 && l.front() == 0 && l.back() == 2);
	
	int sum = 0;
	for(auto it = l.begin(); it != l.end(); ++it) sum += *it;
	assert(sum == 3);
	
	auto it = l.begin(); ++it;
	l.insert(it, 99);                              // [0,99,1,2]
	assert(l.size() == 4);
	l.erase(it);                                    
	
	R_list<int> m; m.push_back(7); m.push_back(8);
	l.splice(m);
	assert(l.size() == 5 && m.empty());
	assert(l.back() == 8);
	
	R_list<int> l2(l);                            
	assert(l2.size() == 5);
	R_list<int> l3(std::move(l));                 
	assert(l3.size() == 5 && l.empty());
}

int main(){                 
	test_list();
	return 0;
}