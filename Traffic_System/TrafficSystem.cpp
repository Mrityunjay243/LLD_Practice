#include "TrafficSystem.hpp"
#include <iostream>
#include <algorithm>
 
TrafficSystem::TrafficSystem() : intersectionCounter(1), signalIdCounter(1) { };

TrafficSystem::~TrafficSystem(){
    for (auto intersection: intersections){
        delete intersection; 
    }
}

Intersection* TrafficSystem::createIntersection() {
    std::string intersectionId = generateIntersectionId(); 
    Intersection* intersection = new Intersection(intersectionId); 
    intersections.push_back(intersection); 
    return intersection; 
}

void TrafficSystem::removeIntersection(const std::string& intersectionId){
    auto it = std::find_if(intersections.begin(), intersections.end(), 
    [intersectionId](Intersection* intersection){
        return intersection->getIntersectionId() == intersectionId; 
    });

    if (it != intersections.end()){
        delete *it; 
        intersections.erase(it); 
    }
}

Signal* TrafficSystem::addSignal(const std::string& intersectionId, int greenDuration, int yellowDuration, int redDuration){
    Intersection* intersection = findIntersection(intersectionId); 
    if (!intersection) return nullptr; 

    std::string signalId = generateSignalId(); 
    Signal* signal = new Signal(signalId, greenDuration, yellowDuration, redDuration); 
    intersection->addSignal(signal); 
    return signal; 
}


void TrafficSystem::removeSignal(const std::string& intersectionId, const std::string& signalId){
    if (Intersection* intersection = findIntersection(intersectionId)){
        intersection->removeSignal(signalId); 
    }
}

void TrafficSystem::updateSystem(int timeElapsed){
    for (auto intersection: intersections){
        intersection->updateSginals(timeElapsed); 
    }
}


void TrafficSystem::setIntersectionStatus(const std::string& intersectionId, bool operational){
    if (Intersection* intersection = findIntersection(intersectionId)){
        intersection->setOperational(operational); 
    }
}

void TrafficSystem::syncronizeIntersection(const std::string& intersectionId){
    if (Intersection* intersection = findIntersection(intersectionId)){
        intersection->syncronizeSignals(); 
    }
}

void TrafficSystem::displaySystemStatus() const {
    std::cout << "\nTraffic System Status: " << std::endl; 
    std::cout << "Number of intersections: " << intersections.size() << std::endl; 

    for (const auto& intersection: intersections){
        intersection->displayStatus(); 
    }
}

Intersection* TrafficSystem::findIntersection(const std::string& intersectionId) const{
    auto it = std::find_if(intersections.begin(), intersections.end(), 
    [intersectionId] (Intersection* intersection){
        return intersection->getIntersectionId() == intersectionId; }); 

    return it != intersections.end() ? *it : nullptr; 
}

std::string TrafficSystem::generateIntersectionId() {
    return "I" + std::to_string(intersectionCounter++); 
}

std::string TrafficSystem::generateSignalId() {
    return "S" + std::to_string(signalIdCounter++);
}
