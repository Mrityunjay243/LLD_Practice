#include <iostream>
#include "shared_ptr.hpp"

struct Test{
    int val;
    Test(int x): val(x) {std::cout<<"Constructed";}
    ~Test() { std::cout<<"Destroyed" << val << std::endl; }
    void greet() const { std::cout<< "Hello from" << val << std::endl; }
};

int main(){
    {
        SharedPtr<Test> p1(new Test(24)); 
        std::cout<< "Use_count: " << p1.use_count() << std::endl; 
        {
            SharedPtr<Test> p2 = p1; 
            std::cout<< "Use_count after copy: " << p1.use_count() << std::endl; 

            p2->greet(); 
            (*p1).greet(); // accessing through dereferencing
        }

        std::cout<< "use_count after p2 is destroyed: " << p1.use_count() << std::endl;  
    }

    std::cout<< "All done"; 
    return 0; 
}
