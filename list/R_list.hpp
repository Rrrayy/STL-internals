#pragma once
#include <cstddef>
#include <utility>

template<typename T>
struct node{
    T data;
    node* prev;
    node* next;
};

template<typename T>
class R_list{
    private:
        using node_t = node<T>;
        node_t* sentinel_; //哨兵节点
        size_t size_;
    public:
        
        class iterator{
            private:
                node_t* ptr;
            public:
                iterator(node_t* p) : ptr(p) {}
                
                iterator& operator++(){ 
                    ptr = ptr->next; 
                    return *this; 
                }

                iterator operator++(int){ 
                    iterator tmp=*this; 
                    ++*this; 
                    return tmp; 
                }  // 后置++
                
                iterator& operator--(){ 
                    ptr = ptr->prev; 
                    return *this; 
                }      
                
                T& operator*() const { 
                    return ptr->data; 
                }
                
                T* operator->() const { 
                    return &ptr->data; 
                }
                
                bool operator==(const iterator& o) const { 
                    return ptr == o.ptr; 
                }

                bool operator!=(const iterator& o) const { 
                    return ptr != o.ptr; 
                }
                
                operator node_t*() const { 
                    return ptr; 
                }   // 隐式转回裸指针
        };

        R_list(): size_(0){
            sentinel_ = new node_t{};
            sentinel_->prev = sentinel_;
            sentinel_->next = sentinel_;
        }  
        
        R_list(const R_list& o) : R_list(){
            for(node_t* cur = o.sentinel_->next; cur != o.sentinel_; cur = cur->next){
                push_back(cur->data);
            }
        }

        R_list(R_list&& o) noexcept : R_list(){
            std::swap(sentinel_, o.sentinel_);
            std::swap(size_, o.size_);
        }
        
        void swap (R_list& o) noexcept{
            std::swap(sentinel_, o.sentinel_);
            std::swap(size_, o.size_);
        }

        R_list& operator=(const R_list& o){
            if(this != &o){
                R_list tmp(o);
                swap(tmp);
            }
            return *this;
        }

        // R_list& operator=(R_list&& o) noexcept{
        //     if(this != &o){
        //         clear();
        //         std::swap(sentinel_, o.sentinel_);
        //         std::swap(size_, o.size_);
        //     }
        //     return *this;
        // }

        R_list& operator=(R_list&& o) noexcept{
            if(this != &o){
                clear();
                std::swap(sentinel_, o.sentinel_);
                std::swap(size_, o.size_);
            }
            return *this;
        }

        ~R_list(){
            clear();
            delete sentinel_;
        }
        
        void clear(){
            node_t* cur = sentinel_->next;
            while(cur != sentinel_){
                node_t* nxt = cur->next;
                delete cur;
                cur = nxt;
            }
            sentinel_ ->prev = sentinel_;
            sentinel_ ->next = sentinel_;
            size_ = 0;
        }

        size_t size() const {return size_;}
        bool empty() const { return size_==0;}

        // 在pos前插一个新结点
        void insert_node(node_t* pos , node_t* n){
            n->prev = pos->prev;
            n->next =pos;
            pos->prev->next =n ;
            pos->prev = n;
            ++size_;
        }
        
        void push_back(const T& val){
            node_t* n = new node_t{val ,nullptr ,nullptr };
            insert_node(sentinel_ , n);
        }
        
        void push_front(const T& val){
            node_t* n= new node_t {val ,nullptr, nullptr};
            insert_node(sentinel_ ->next , n);
        }

        node_t* insert(node_t* pos , const T& val){
            node_t* n = new node_t{val, nullptr ,nullptr};
            insert_node(pos , n);
            return n;
        }

        node_t* erase_node(node_t* pos){
            node_t* nxt = pos->next;
            pos->prev->next = pos->next;
            pos->next->prev = pos->prev;
            delete pos;
            --size_;
            return nxt;
        }
        
        void pop_front(){
            if(!empty())
                erase_node(sentinel_->next);
        }

        void pop_back(){
            if(!empty())
                erase_node(sentinel_->prev);
        }

        node_t* erase(node_t* pos){
            return erase_node(pos);
        }
    
        iterator begin(){
            return sentinel_->next;
        }

        iterator end(){
            return sentinel_;
        }
    
        T& front(){
            return sentinel_->next->data; 
        }
        
        T& back(){
            return sentinel_->prev->data;
        }

        //把other加到this尾部
        void splice(R_list& other){
            if(other.empty())
                return ;
            node_t* t_tail = sentinel_->prev;
            node_t* o_head = other.sentinel_->next;
            node_t* o_tail = other.sentinel_->prev;

            t_tail->next = o_head;
            o_head->prev = t_tail;
            o_tail->next = sentinel_;
            sentinel_->prev = o_tail;
            size_ += other.size_;
            other.sentinel_->next = other.sentinel_;
            other.sentinel_->prev = other.sentinel_;   
            other.size_ = 0;
        }

};
