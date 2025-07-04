**Requirements**
- The system should allow guests to book rooms, check in and check out
- The system should manage different types of rooms
- The system should handle room availability 
- The system shuold allow hotel staff to manage guest info, room assignments, and billing 
- The system should support multiple payment methods
- The system should handle concurrent bookings and ensure data consistency 

**Classes, Interfaces, and Enums**
- The Guest class represents a guest of hotel, with properties such as ID, name, email, and phone number
- The Room class represents a room in the hotel, with properties like ID, roomtype, price, and status
- The RoomType enum represents different types rooms available in the hotel
- The RoomStatus enum represents the status of a room, which can be available, booked or occupied 
- The Booking class represents the reservation made by a guest
- The HotelManagementSystem is the central component of the system 
- main.cpp is the entry point to our program 
