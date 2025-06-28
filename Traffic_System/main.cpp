#include "TrafficSystem.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main(){
    TrafficSystem system; 

    Intersection* intersection1 = system.createIntersection();

    // Adding singals to the intersection
    Signal* signal1 = system.addSignal(intersection1->getIntersectionId(), 20, 5, 25); 
    Signal* singal2 = system.addSignal(intersection1->getIntersectionId(), 20, 5, 25); 

    // Displaying initial status 
    system.displaySystemStatus(); 

    
    // Simulating traffic for a few cycles 
    for( int i=0; i<10; i++){
        std::cout<<"\nTime step " << i+1 << ":" << std::endl; 
        system.updateSystem(5); 
        system.displaySystemStatus(); 
        std::this_thread::sleep_for(std::chrono::seconds(1)); 
    }

    return 0; 

}