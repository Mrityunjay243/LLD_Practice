#ifndef CUSTOMER_HPP
#define CUSTOMER_HPP

#include <string>

class Customer{
    private:
        std::string customerId; 
        std::string name; 
        std::string contactNumber; 

    public:
        Customer(std::string& customerId, std::string name, std::string contactNumber); 

        std::string getCustomerId() const; 
        std::string getCustomerName() const; 
        std::string getContactNumber() const; 

        void displayInfo() const; 
}; 

#endif