#ifndef R_VECTOR_HPP
#define R_VECTOR_HPP

#include<cstddef>
#include<new>
#include<utility>
#include<stdexcept>

template<typename T>
class R_vector{
    private:
        T* _start;
        T* _finish;
        T* _end_of_storage;
        void grow(){
            size_t old_size = size();
            size_t old_cap  = capacity();
            size_t new_cap = old_cap ? old_cap * 3 / 2 : 1;
            if(new_cap <= old_cap) new_cap = old_cap + 1;
            T* new_start = new T[new_cap];
            for(size_t i=0;i<old_size;++i)
                new_start[i] = _start[i];
            delete[] _start;
            _start = new_start;
            _finish = _start + old_size;
            _end_of_storage = _start + new_cap;
        }
    public:
        R_vector(): _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}
        ~R_vector() { delete[] _start;  }
        
        size_t size() const {return _finish-_start;}
        size_t capacity() const {return _end_of_storage-_start;}

        void push_back(const T& val){
            if(size() == capacity())
                grow();
            *_finish = val;
            ++_finish;    
        }
        
        template<typename... Args>
        void emplace_back(Args&&... args){
            if(size() == capacity())
                grow();
            new(_finish) T(std::forward<Args>(args)...);
            ++_finish;
        }
        
        void insert(size_t pos , const T& val){
            if(pos >size())
                throw std::out_of_range("R_vector::insert");
            if(size() == capacity())    
                grow();
            for(size_t i=size();i>pos;i--)
                _start[i] = _start[i-1];
            _start[pos] = val; 
            _finish++;
        }

        void erase(size_t pos){
            if(pos>=size())
                throw std::out_of_range("my_vector::erase");
            for(size_t i = pos; i+1<size() ;++i){
                _start[i]=_start[i+1];
            }
            --_finish;
        }
        
        void pop_back(){
            if(size() >0 )
                --_finish;
        }

        T* data(){
            return _start;
        }

        T& operator[](size_t i){
            return _start[i];
        }
        
        T& at(size_t i){
            if (i>= size()) 
                throw std::out_of_range("R_vector::at");
            return _start[i];
        }

};

#endif
