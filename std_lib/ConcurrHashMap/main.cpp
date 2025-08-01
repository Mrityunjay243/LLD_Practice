#include <iostream>
#include <thread>
#include "ConcurrentHashMap.hpp"

void worker(ConcurrentHashMap<int, std::string>& map, int id){
    for (int i=0; i<5; i++){
        int key = id*10 + i; 
        map.insert(key, "val_" + std::to_string(key)); 
    }

    for (int i=0; i<5; i++){
        int key = id*10 + i; 
        auto val = map.get(key); 
        if (val){
            std::cout<< "Thread " << id << " got: " << key << " -> " << *val << "\n"; 
        }
    }
}

int main(){

    ConcurrentHashMap<int, std::string> mp; 

    std::thread t1(worker, std::ref(mp), 1); 
    std::thread t2(worker, std::ref(mp), 2); 

    t1.join(); 
    t2.join(); 

    return 0; 
}