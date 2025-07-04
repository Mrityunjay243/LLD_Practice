#ifndef ROOM_HPP
#define ROOM_HPP

#include <string>

enum class RoomType{
    STANDARD, 
    DELUXE, 
    SUITE
}; 

enum class RoomStatus{
    AVAILABLE, 
    OCCUPIED, 
    UNDER_MAINTAINENCE
}; 

class Room{
    private:
        std::string roomNumber; 
        RoomType type; 
        RoomStatus status; 
        double pricePerNight; 
        int capacity; 

    public: 

        Room(std::string roomNumber, RoomType type, double price, int capacity); 

        std::string getRoomNumber() const; 
        RoomType getType() const; 
        RoomStatus getStatus() const; 
        double getRoomPrice() const; 
        int getCapacity() const; 
        
        void setStatus(RoomStatus status); 
        void displayInfo() const; 

};

#endif