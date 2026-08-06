#pragma once                       
#include <cstddef>                  
#include <cstring>                 
#include <stdexcept>                // std::out_of_range

class R_string{

	union data{                              // union：所有成员共享一块内存，大小=最大成员
		char buf[16];                        // 短字符串存这
		struct heap_t{                       // 堆指针+容量
			char* ptr;
			size_t cap;
		} heap;
		data() : buf{} {}                    // 默认先激活 buf 视图
	};

	data data_;
	size_t size_ = 0;
	static constexpr size_t SSO_MAX = 15;    // 最大短字符串长度

	bool is_sso() const { return size_ <= SSO_MAX; }   

	void release_heap(){                     // 释放堆内存
		if(data_.heap.ptr) 
            delete[] data_.heap.ptr;
	}

    void init_from(const char* s, size_t n){ // 通用初始化
		size_ = n;
		if(is_sso()){
			memcpy(data_.buf, s, n);         
			data_.buf[n] = '\0';             // 补结束符
		}else{
			char* p = new char[n + 1];       // 堆分配
			memcpy(p, s, n);
			p[n] = '\0';
			data_.heap.ptr = p;
			data_.heap.cap = n;
		}
	}

public:
	R_string() = default;                   

	R_string(const char* s){               
		size_t n = strlen(s);
		init_from(s, n);
	}

	~R_string(){                            
		if(!is_sso()) release_heap();        
	}

    R_string(const R_string& o){           
		size_ = o.size_;
		if(o.is_sso()){
			memcpy(data_.buf, o.data_.buf, size_ + 1);   
		}else{
			char* p = new char[o.data_.heap.cap + 1];  
			memcpy(p, o.data_.heap.ptr, size_ + 1);
			data_.heap.ptr = p;
			data_.heap.cap = o.data_.heap.cap;
		}
	}

	R_string(R_string&& o) noexcept{       // 移动构造
		size_ = o.size_;
		if(o.is_sso()){
			memcpy(data_.buf, o.data_.buf, size_ + 1);  
		}else{
			data_.heap = o.data_.heap;       
			o.data_.heap.ptr = nullptr;      // 原对象置空，防止它析构时释放
			o.size_ = 0;                     // o 从此变空串（SSO 态）
		}
	}

	R_string& operator=(const R_string& o){   // 拷贝赋值
		if(this != &o){
			if(!is_sso()) release_heap();       
			size_ = o.size_;
			if(o.is_sso()){
				memcpy(data_.buf, o.data_.buf, size_ + 1);
			}else{
				char* p = new char[o.data_.heap.cap + 1];
				memcpy(p, o.data_.heap.ptr, size_ + 1);
				data_.heap.ptr = p;
				data_.heap.cap = o.data_.heap.cap;
			}
		}
		return *this;
	}

    const char* c_str() const{              
		return is_sso() ? data_.buf : data_.heap.ptr;
	}

	const char* data() const { return c_str(); }
	size_t size() const { return size_; }
	size_t capacity() const { return is_sso() ? SSO_MAX : data_.heap.cap; }
	bool empty() const { return size_ == 0; }

	char& operator[](size_t i){ return is_sso() ? data_.buf[i] : data_.heap.ptr[i]; } 

	char& at(size_t i){ 
        if(i >= size_) 
            throw std::out_of_range("R_string"); 
        return is_sso() ? data_.buf[i] : data_.heap.ptr[i]; 
    }

	void clear(){                          
		if(!is_sso()) 
            release_heap();
		size_ = 0;                          
	}

	void push_back(char c){                
		if(is_sso()){
			if(size_ < SSO_MAX){             // 短态且还有空位：直接写 buf
				data_.buf[size_++] = c;
				data_.buf[size_] = '\0';
			}else{                           // 短态满了切堆：拷贝16字节出来再追加
				char* p = new char[32];     
				memcpy(p, data_.buf, size_); // 把 buf 里的数据搬进堆
				p[size_++] = c;
				p[size_] = '\0';
				data_.heap.ptr = p;
				data_.heap.cap = 32;
			}
		}else{                             
			if(size_ < data_.heap.cap){
				data_.heap.ptr[size_++] = c;
				data_.heap.ptr[size_] = '\0';
			}else{
				char* p = new char[data_.heap.cap * 2];   // 翻倍扩容
				memcpy(p, data_.heap.ptr, size_);
				delete[] data_.heap.ptr;     // 释放旧的
				p[size_++] = c;
				p[size_] = '\0';
				data_.heap.ptr = p;
				data_.heap.cap *= 2;
			}
		}
	}

	void append(const char* s){ while(*s) push_back(*s++); }   

	void reserve(size_t n){                  // 预分配（仅堆态有意义）
		if(n <= SSO_MAX || is_sso()) return; 
		if(n <= data_.heap.cap) return;     
		char* p = new char[n + 1];
		memcpy(p, c_str(), size_);           
		p[size_] = '\0';
		if(!is_sso()) release_heap();
		data_.heap.ptr = p;
		data_.heap.cap = n;
	}

    bool operator==(const R_string& o) const{
		if(size_ != o.size_) 
            return false;
		return memcmp(c_str(), o.c_str(), size_) == 0;   
	}

	bool operator<(const R_string& o) const{
		return strcmp(c_str(), o.c_str()) < 0;           
	}
};