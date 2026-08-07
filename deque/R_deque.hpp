#pragma once
#include<cstddef>
#include<utility>

template<typename T>
class R_deque;

template<typename T>
class iterator{
    T**node_;
    T* cur_;
    T* first_;
    T* last_;
    friend class R_deque<T>;
public:
    iterator(T** n, T*c ,T*f ,T*l)
        :node_(n),cur_(c),first_(f),last_(l){}
    
    iterator& operator++(){
        size_t bs = last_ - first_; //block size 
        ++cur_;
        if(cur_ == last_){
            ++node_;
            first_ = *node_;
            last_ = first_ + bs;
            cur_ = first_;
        }
        return *this;
    }

    iterator operator++(int){
        iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    iterator& operator--(){
        size_t bs = last_ - first_;  
        if(cur_ == first_){
            --node_;
            first_ = *node_;
            last_ = first_ + bs;
            cur_ = last_;
        }
        --cur_;
        return *this;
    }
    
    iterator operator--(int){
        iterator tmp = *this;
        --(*this);
        return tmp;
    }

    T& operator*() const{
        return *cur_;
    }

    T* operator->() const{
        return cur_;
    }

    bool operator==(const iterator& rhs) const{
        return cur_ == rhs.cur_;
    }

    bool operator!=(const iterator& rhs) const{
        return !(*this == rhs);
    }

};

template<typename T>
class R_deque{
    using iterator = ::iterator<T>;
    static const size_t block_size = 4; //gcc's real size is 512 bits
    T** map_;
    size_t map_cap_;
    size_t map_first_;
    size_t size_;
    iterator start_;
    iterator finish_;
public:

    R_deque()
        :map_(nullptr),map_cap_(0),map_first_(0),size_(0),
        start_(nullptr,nullptr,nullptr,nullptr),
        finish_(nullptr,nullptr,nullptr,nullptr)
    {}

    ~R_deque(){
        clear();
        delete[] map_;
    }   

    void clear(){
        if(start_.node_ == nullptr) return;
        for(T** p = start_.node_; p <= finish_.node_ && p != nullptr; ++p){
            delete[] *p;
        }
        size_ = 0;
        map_first_ = 0;
        start_ = finish_ = iterator(nullptr,nullptr,nullptr,nullptr);
    }

    bool empty() const{
        return size_ == 0;
    }
    
    size_t size() const{
        return size_;
    }

    void push_back(const T& val){
        if(empty()){
            ensure_map(1);
            T* buf = new T[block_size];
            map_[map_first_] = buf;
            start_ = iterator(map_ + map_first_, buf, buf, buf + block_size);
            finish_ = start_;
            *finish_.cur_ = val;
            ++finish_.cur_;
            size_= 1;
            return;  
        }
        if(finish_.cur_ == finish_.last_){
            ensure_map(1);
           ++finish_.node_;
           *finish_.node_ = new T[block_size];
            finish_.first_ = *finish_.node_;
            finish_.last_ = finish_.first_ + block_size;
            finish_.cur_ = finish_.first_;
        }
        *finish_.cur_ = val;
        ++finish_.cur_;
        ++size_;
    }

    void push_front(const T& val){
        if(empty()){
            ensure_map(1);
            T* buf = new T[block_size];
            map_[map_first_] = buf;
            start_ = iterator(map_ + map_first_, buf, buf, buf + block_size);
            finish_ = start_;
            *finish_.cur_ = val;
            ++finish_.cur_;
            size_= 1;
            return;  
        }
        if(start_.cur_ == start_.first_){
            ensure_map(1);
           --start_.node_;
           *start_.node_ = new T[block_size];
            start_.first_ = *start_.node_;
            start_.last_ = start_.first_ + block_size;
            start_.cur_ = start_.last_-1;
        } else {
            --start_.cur_;
        }
        *start_.cur_ = val;
        ++size_;
    }
    
    void pop_back(){
        if(empty()) return;
        if(finish_.cur_ == finish_.first_){
            delete[] *finish_.node_;
            --finish_.node_;
            finish_.first_ = *finish_.node_;
            finish_.last_ = finish_.first_ + block_size;
            finish_.cur_ = finish_.last_-1;
        } 
        --finish_.cur_;
        --size_;
        if(size_==0){
            delete[] start_.first_;
            start_ = finish_ = iterator(nullptr,nullptr,nullptr ,nullptr);
            map_first_ = map_cap_/2 -1;
        }
    }

    void pop_front(){
        if(empty()) return ;
        if(start_.cur_ +1 ==start_.last_){
            delete[] *start_.node_;
            ++start_.node_;
            start_.first_ = *start_.node_;
            start_.last_ = start_.first_ + block_size;
            start_.cur_ = start_.first_;
        } else {
            ++start_.cur_;
        }
        --size_;
        if(size_==0){
            delete[] finish_.first_;
            start_ = finish_ = iterator(nullptr,nullptr,nullptr ,nullptr);
            map_first_ = map_cap_/2 -1;
        }
    }

    T& operator[](size_t i){
        size_t pos = start_.cur_ - start_.first_ + i;
        size_t block = pos / block_size;
        size_t offset = pos % block_size;
        return map_[start_.node_-map_+block][offset];
    }

    const T& operator[](size_t i) const{     
        size_t pos = start_.cur_ - start_.first_ + i;
        size_t block = pos / block_size;
        size_t offset = pos % block_size;
        return map_[start_.node_ - map_ + block][offset];
    }
    iterator begin(){
        return start_;
    }

    iterator end(){
        return finish_;
    }

    R_deque(const R_deque& rhs): R_deque(){
        for(size_t i = 0; i < rhs.size(); ++i){
            push_back(rhs[i]);
        }
    }

    R_deque(R_deque&& rhs) : R_deque(){
        std::swap(map_, rhs.map_);
        std::swap(map_cap_, rhs.map_cap_);
        std::swap(map_first_, rhs.map_first_);
        std::swap(size_, rhs.size_);
        std::swap(start_, rhs.start_);
        std::swap(finish_, rhs.finish_);
    }

    R_deque& operator= (const R_deque& rhs){
        if(this != &rhs){
            R_deque tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    R_deque& operator= (R_deque&& rhs){
        if(this != &rhs){
            std::swap(map_, rhs.map_);
            std::swap(map_cap_, rhs.map_cap_);
            std::swap(map_first_, rhs.map_first_);
            std::swap(size_, rhs.size_);
            std::swap(start_, rhs.start_);
            std::swap(finish_, rhs.finish_);
        }
        return *this;
    }
    
    void swap(R_deque& rhs){
        std::swap(map_, rhs.map_);
        std::swap(map_cap_, rhs.map_cap_);
        std::swap(map_first_, rhs.map_first_);
        std::swap(size_, rhs.size_);
        std::swap(start_, rhs.start_);
        std::swap(finish_, rhs.finish_);
    }

private:
    size_t blocks() const{
        if(start_.node_ == nullptr) return 0;
        return finish_.node_ - start_.node_ + 1;
    }

    void ensure_map(size_t need){
        if(map_cap_ == 0){
            map_cap_ = 8;
            map_ = new T*[map_cap_];
            map_first_ = map_cap_/2 -1 ;
            return ;
        }
        size_t left = map_first_;
        size_t right = map_cap_ - (map_first_ + blocks());
        if(left >= need&& right >= need) return;
        T** old_map = map_;
        size_t old_first = map_first_;
        size_t old_blocks = blocks();

        size_t new_cap = map_cap_ *2;
        T** new_map = new T*[new_cap];
        size_t new_first = new_cap/2 - old_blocks/2;
        for(size_t i = 0; i < old_blocks; ++i){
            new_map[new_first + i] = old_map[old_first + i];
        }
        delete[] old_map;
        map_ = new_map;
        map_first_ = new_first;
        map_cap_ = new_cap;

        start_.node_  = map_ + new_first + (start_.node_  - old_map - old_first);
        finish_.node_ = map_ + new_first + (finish_.node_ - old_map - old_first);
    }

}; 