#include <iostream>
#include "Vector.hpp"

int main(){

    Vector<int> v; 
    v.p_b(1); 
    v.p_b(2); 
    v.p_b(3); 

    std::cout<<"Vector elements" << std::endl; 
    for (int i=0; i<v.size(); i++){
        std::cout << v[i] << std::endl; 
    }

    std::cout << "Size: " << v.size() << ", capacity: " << v.cap() << std::endl; 
    v.pop(); 
    std::cout<< "After popping, size: " << v.size() << std::endl; 

    v.resize(5, 99); 
    for (int i=0; i<v.size(); i++){
        std::cout << v[i] << std::endl; 
    }

    return 0; 
}