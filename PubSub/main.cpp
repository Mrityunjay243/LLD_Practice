#include "PubSubSystem.hpp"
#include <iostream>
#include <thread>
#include <chrono>


int main() {

    PubSubSystem system; 

    // Create topics 
    Topic* tech = system.createTopic("Tech", "Tech updates"); 
    Topic* sports = system.createTopic("Sports", "Sports and news scores"); 
    Topic* weather = system.createTopic("Weather", "Weather updates"); 

    std::cout << "Initial Topics: "; 
    system.displayTopics(); 

    Subscriber* sub1 = system.addSubscriber("Mj"); 
    Subscriber* sub2 = system.addSubscriber("Mj2");
    Subscriber* sub3 = system.addSubscriber("Mj3"); 


    // Subscribe to topics 
    system.subscribe(sub1->getId(), "Tech"); 
    system.subscribe(sub1->getId(), "Weather");
    system.subscribe(sub2->getId(), "Sports");
    system.subscribe(sub2->getId(), "Tech"); 
    system.subscribe(sub3->getId(), "Tech"); 
    system.subscribe(sub3->getId(), "Sports"); 

    // Publish Messages
    std::cout << "Publishing Messages: " << std::endl;  

    system.publish("Tech", "New LLM breakthrough"); 
    system.publish("Sports", "Real Madrind wins fifa club world cup");
    system.publish("Weather", "Sunny Weather exprected"); 
    
    // Small delay to simulate passing 
    std::this_thread::sleep_for(std::chrono::seconds(1)); 

    // Display messages for each subscriber
    std::cout<< "\nChecking messages for subs: " << std::endl; 
    system.displaySubscriberMessages(sub1->getId()); 
    system.displaySubscriberMessages(sub2->getId()); 
    system.displaySubscriberMessages(sub3->getId()); 

    // Unsubscribe and test 
    std::cout << "\nUnsubscribing Mj from weather." << std::endl; 
    system.unsubscribe(sub1->getId(), "weather"); 

    system.publish("Weather", "Storm Warning"); 

    std::cout<<"Check for Mj's updates: " << std::endl; 
    system.displaySubscriberMessages(sub1->getId()); 

    return 0; 
}