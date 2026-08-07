#include"R_forward_list.hpp"
#include<utility>
#include<cassert>

void test(){
    R_forward_list<int> l;
    assert(l.empty());

    l.push_front(3); l.push_front(2); l.push_front(1);
    assert(!l.empty()&&l.front()==1);

    int cnt= 0 , sum=0;
    for(auto it = l.begin(); it!=l.end(); ++it){
        sum += *it;
        ++cnt;
    }
    assert(cnt==3&&sum==6);

    l.insert_after(l.begin(),9);
    assert(l.front()==1);

    l.erase_after(l.begin());
    int sum2=0;
    for(auto it = l.begin(); it!=l.end(); ++it){
        sum2 += *it;            
    }
    assert(sum2==6);

    auto cur = l.begin();
    l.erase_after(cur);
    l.erase_after(cur);
    assert(l.front()==1);
    l.erase_after(cur);
    l.pop_front();
    assert(l.empty());  

    l.push_front(7);
    l.insert_after(l.before_begin(),8);
    assert(l.front()==8);

    R_forward_list<int> l2(l);
    int n=0;
    for(auto it = l2.begin(); it!=l2.end(); ++it){
        ++n;
    }
    assert(n==2&&l2.front()==8 );

    R_forward_list<int> l3(std::move(l2));
    assert(l2.empty()&&l3.front()==8);
}

int main(){
    test();
    return 0;
}