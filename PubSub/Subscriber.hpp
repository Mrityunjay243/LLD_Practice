#ifndef SUBSCRIBER_HPP
#define SUBSCRIBER_HPP

#include <string>
#include <vector>
#include "Message.hpp"

class Subscriber {
    private: 
        std::string id; 
        std::string name; 
        std::vector<Message> messageQueue; 
        bool active; 

    public:
        Subscriber(std::string id, std::string name); 

        std::string getId() const; 
        std::string getName() const; 
        bool isActive() const; 
        const std::vector<Message>& getMessage() const; 

        void receiveMessage(const Message& message); 
        void displayMessage() const; 
        void clearMessage(); 
        void setActive(bool status); 
        void displayInfo() const; 
};


#endif 