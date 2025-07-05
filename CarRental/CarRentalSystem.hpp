#ifndef CAR_RENTAL_SYSTEM_HPP
#define CAR_RENTAL_SYSTEM_HPP

#include <vector>
#include <string>
#include "Car.hpp"
#include "Rental.hpp"
#include "Customer.hpp"

class CarRentalSystem{
    public:
        std::vector<Car*> cars; 
        std::vector<Customer*> customers; 
        std::vector<Rental*> rentals; 
        int rentalIdCounter; 

    private:
        CarRentalSystem(); 
        ~CarRentalSystem(); 

        void addCar(Car* car); 
        void addCustomer(Customer* customer); 
        std::string rentCar(Car* car, Customer* customer, std::string startDate, int days); 
        bool returnCar(std::string rentalId); 

        void displayAvailableCars() const; 
        void displayRentals() const; 
        void displayCustomers() const; 

        Car* findCar(std::string carId) const; 
        Customer* findCustomer(std::string customerId) const; 
        Rental* findRental(std::string rentalId) const; 
}; 

#endif