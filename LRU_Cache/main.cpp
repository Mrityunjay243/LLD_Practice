#include <iostream>
#include "LRUCache.hpp"

using namespace std; 

int main() {
    LRUCache cache(2); 
    cache.put(1, 1); 
    cache.put(2, 2);

    cout<< "get(1): " << cache.get(1) << endl; 
    
    cache.put(3,3); 
    cout<< "get(2): " << cache.get(2) << endl; 
    cache.put(4,4); 
    cout << "get(1): " << cache.get(1) << endl; 
    cout << "get(3): " << cache.get(3) << endl; 
    cout << "get(4): " << cache.get(4) << endl; 

    return 0; 
}