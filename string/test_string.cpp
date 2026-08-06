#include "R_string.hpp"       
#include <cassert>
#include <cstring>
#include <utility>

void test_string(){
	R_string s1("hi");                              
	assert(s1.size() == 2 && strcmp(s1.c_str(), "hi") == 0);

	R_string s2("hello world hello world hello");   
	assert(s2.size() == 29 && s2.capacity() >= 29);

	R_string s3(s1);                               
	R_string s4(s2);                               
	assert(strcmp(s3.c_str(), "hi") == 0 && strcmp(s4.c_str(), s2.c_str()) == 0);

	R_string s5(std::move(s2));                    
	assert(s5.size() == 29 && s2.empty());           

	R_string s6;                                    
	for(int i = 0; i < 20; i++) s6.push_back('a' + i % 26);
	assert(s6.size() == 20);

	s6 = "x";                                        
	assert(s6.size() == 1 && strcmp(s6.c_str(), "x") == 0);

	assert(s6 == R_string("x") && R_string("ab") < R_string("ac"));
	assert(s1.at(1) == 'i');
	
}

int main(){                 
	test_string();
	return 0;
}