#pragma once
#include<cstddef>
#include<utility>

template<typename T>
struct node{
    T data;
    node* next;
};
template<typename T>
class R_forward_list;

template<typename T>
class iterator{
    node<T>* ptr;
    friend class R_forward_list<T>;
public:
    iterator(node<T>* p) :ptr(p) {}
    iterator& operator++(){
        ptr=ptr->next;
        return *this;
    }
    iterator operator++(int){
        iterator tmp = *this;
        ptr = ptr->next;
        return tmp;
    }
    T& operator*() const{
        return ptr->data;
    }
    T* operator->() const{
        return &(ptr->data);
    }
    bool operator==(const iterator& rhs) const{
        return ptr == rhs.ptr;
    }
    bool operator!=(const iterator& rhs) const{
        return ptr != rhs.ptr;
    }

};

template<typename T>
class R_forward_list{
    using node_t = node<T>;
    node_t* head_; //dummy head node
public:
    using iterator = ::iterator<T>; // 全局限定符：避开类内名字遮蔽（iterator 与类同名）

    R_forward_list(): head_(new node_t{}){
        head_->next = nullptr;
    }

    R_forward_list(const R_forward_list& rhs): R_forward_list(){
        node_t* prev = head_;
        for(node_t* cur = rhs.head_->next; cur; cur = cur->next){
            prev->next = new node_t{cur->data,nullptr};
            prev = prev->next;
        }
    }

    R_forward_list(R_forward_list&& rhs) noexcept: R_forward_list(){
        std::swap(head_,rhs.head_);
    }

    ~R_forward_list(){
        clear();
        delete head_;
    }

    R_forward_list& operator=(const R_forward_list& rhs){
        if(this != rhs){
            R_forward_list tmp(rhs);
            swap(tmp);
        }
        return *this;
    }

    R_forward_list& operator=(R_forward_list&& rhs) noexcept{
        if(this!=rhs){
            clear();
            std::swap(head_,rhs.head_); 
        }
        return *this;
    }

    void clear(){
        node_t* cur = head_->next;
        while(cur){
            node_t* nxt = cur->next;
            delete cur;
            cur = nxt;
        }
        head_->next = nullptr;
    }

    bool empty() const{
        return head_->next == nullptr;
    }

    void push_front(const T& val){
        head_->next = new node_t {val, head_->next};
    }

    void pop_front(){
        if(empty())
            return;
        node_t * tmp = head_->next;
        head_->next = tmp->next;
        delete tmp;
    }

    T& front(){
        return head_->next->data;        
    }

    //单向链表只能后插
    iterator insert_after(iterator pos,const T& val){
        node_t* p = pos.ptr;
        node_t* n = new node_t{val,p->next};
        p->next=n;
        return iterator(n);
    }
    //删除pos的后继
    iterator erase_after(iterator pos){
        node_t* p =pos.ptr;
        node_t* tmp=p->next;
        if(!tmp)
            return iterator(nullptr);
        p->next=tmp->next;
        delete tmp;
        return iterator(p->next);
    }

    iterator begin(){
        return iterator(head_->next);
    }
    iterator end(){
        return iterator(nullptr);
    }
    iterator before_begin(){
        return iterator(head_);
    }

    void swap(R_forward_list& rhs) noexcept{
        std::swap(head_,rhs.head_);
    }
};