#include <iostream>
#include <thread> 
#include "LRU.hpp"

void cache_worker(LRUCache<int, std::string>& cache, int id){
    for (int i=0; i<5; i++){
        int key = id*10 + i; 
        cache.put(key, "val_" + std::to_string(key)); 
        std::this_thread::sleep_for(std::chrono::milliseconds(50)); 
        auto result = cache.get(key); 

        if (result){
            std::cout << "Thread " << id << " accessed key " << key << ": " << *result << std::endl;  
        }
    }
}

int main(){

    LRUCache<int, std::string> cache(10); 

    std::thread t1(cache_worker, std::ref(cache), 1); 
    std::thread t2(cache_worker, std::ref(cache), 2); 

    t1.join(); 
    t2.join(); 

    return 0; 
}