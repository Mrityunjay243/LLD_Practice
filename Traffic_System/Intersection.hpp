#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "Signal.hpp"
#include <vector>
#include <string> 

class Intersection {
    private: 
        std::string IntersectionId; 
        std::vector<Signal*> signals; 
        bool isOperational; 

    public: 
        Intersection(std::string intersectionId); 
        bool isWorking() const; 

        void addSignal(Signal* signal); 
        void removeSignal(const std::string& signalId); 
        void updateSginals(int timeElapsed); 
        void setOperational(bool status); 
        void syncronizeSignals(); 
        void displayStatus() const; 

    private:
        Signal* findSignal(const std::string& signalId) const; 
}; 

#endif 

