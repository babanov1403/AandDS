#include <iostream>
using namespace std;
template <bool B, typename T, typename V>
struct conditional{
    using type = T;
};

template <>
struct conditional<true>{
    using type = int;
};

template <bool B, typename T, typename V>
using conditional_tx = typename conditional<B, T, V>::type;
int main(){
    return 0;
}