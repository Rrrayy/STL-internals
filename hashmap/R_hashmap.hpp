#ifndef R_HASHMAP_HPP
#define R_HASHMAP_HPP
 
#include<vector>
#include<list>
#include<utility>
 
template<typename Key , typename Value>
class R_hashmap{
    private:
        //拉链法
        std::vector<std::list<std::pair<Key,Value>>> buckets;
        size_t elem_count = 0;
        float load_fac = 0.75;
        
        size_t hash(const Key& key)const {
            return std::hash<Key>{} (key) % buckets.size();
        }
        
        //扩容重哈希
        void rehash(){
            size_t new_count = buckets.size()*2;
            std::vector<std::list<std::pair<Key,Value>>> new_buckets(new_count);
            for(auto &buk : buckets){
                for(auto &kv : buk){
                    size_t idx = std::hash<Key>{} (kv.first) % new_count;
                    new_buckets[idx].push_back(kv);
                }
            }
            buckets = std::move(new_buckets);
        }    
    public:
        R_hashmap() : buckets(16) {}
        
        void insert(const Key& key , const Value& value){
            size_t index = hash(key);
            for(auto &kv : buckets[index]){
                if(kv.first == key){
                    kv.second = value;
                    return ;
                }
            }
            buckets[index].push_back({key,value}); 
            ++elem_count;
            if(elem_count > buckets.size()*load_fac )
                rehash();
        }

        bool find(const Key& key , Value& value)const{
            size_t index = hash(key);
            for(const auto& kv: buckets[index]){
                if(kv.first == key){
                    value = kv.second;
                    return true;
                }    
            }
            return false;            
        }

        bool erase(const Key& key){
            size_t index = hash(key);
            for(auto it = buckets[index].begin(); it!=buckets[index].end();++it){
                if(it->first == key){
                    buckets[index].erase(it);
                    --elem_count;
                    return true;
                }
            }
            return false;
        }
        
        size_t size() const {return elem_count;}
        size_t bucket_count() const {return buckets.size();}

        std::vector<size_t>chain_len() const{
            std::vector<size_t> lens;
            for(const auto& i : buckets) 
                lens.push_back(i.size());
            return lens;
        }
                                                   
};   
 
#endif 
