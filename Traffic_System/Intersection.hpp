#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "Signal.hpp"
#include <vector>
#include <string> 

class Intersection {
    private: 
        std::string intersectionId; 
        std::vector<Signal*> signals; 
        bool isOperational; 

    public: 
        Intersection(std::string intersectionId); 
        ~Intersection(); 

        std::string getIntersectionId() const; 
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

