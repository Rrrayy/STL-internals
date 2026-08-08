#pragma once 
#include<vector> //底层容器
#include<cstddef>
#include<functional>
#include<utility>

//默认大顶堆 comp(a,b) a<b -> true
template<typename T ,typename Compare>
void push_heap(std::vector<T>& v, Compare comp){
    int i = (int)v.size()-1;
    while(i>0){
        int parent = (i-1)/2;
        if(comp(v[i],v[parent]))
            break;
        std::swap(v[i],v[parent]);
        i=parent;
    }
}

template<typename T ,typename Compare>
void pop_heap(std::vector<T> &v, Compare comp){
    int n = (int)v.size()-1;
    std::swap(v[0],v[n]);
    int i=0;
    while(true){
        int left = 2*i+1;
        int right = 2*i+2;
        int largest = i;
        if(left<n && comp(v[largest],v[left]))
            largest=left;
        if(right<n && comp(v[largest],v[right]))
            largest=right;
        if(largest == i)
            break;
        std::swap(v[i],v[largest]);
        i=largest;
    }
}

template<typename T , typename Compare>
void make_heap(std::vector<T>& v , Compare comp){
    int n = (int)v.size();
    for(int i = n/2 -1; i>=0; --i){//最后一个非叶节点
        int cur =i ;
        while(true){
            int left = 2*cur+1;
            int right = 2*cur+2;
            int largest = cur;
            if(left <n && comp(v[largest],v[left]))
                largest=left;
            if(right<n && comp(v[largest],v[right]))
                largest=right;
            if(largest==cur)
                break;
            std::swap(v[cur],v[largest]);
            cur=largest;
        }
    }
}

template<typename T ,typename Compare = std::less<T>>
class R_priority_queue{
    std::vector<T> c_;
    Compare comp_;
public:
    R_priority_queue() = default;
    explicit R_priority_queue(const Compare& comp)
        :comp_(comp) {}
    
    void push(const T& x){
        c_.push_back(x);
        push_heap(c_,comp_);
    }

    void pop(){
        pop_heap(c_ ,comp_);
        c_.pop_back();
    }

    template<typename... Args>
    void emplace(Args&&... args){
        c_.emplace_back(std::forward<Args>(args)...);
        push_heap(c_,comp_);
    }

    void swap(R_priority_queue& o)noexcept{
        c_.swap(o.c_);
        std::swap(comp_ , o.comp_);
    }

    const T& top() const {
        return c_.front();
    }

    bool empty() const {
        return c_.empty();
    }

    size_t size() const{
        return c_.size();
    }

};