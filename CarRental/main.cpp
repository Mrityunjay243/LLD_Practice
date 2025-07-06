#include "CarRentalSystem.hpp"
#include <iostream>

int main(){

    CarRentalSystem rentalSystem; 

    Car* car1 = new Car("C001", "Toyota", "Camry", 60.0); 
    Car* car2 = new Car("C002", "Honda", "Accord", 71.0); 
    Car* car3 = new Car("C003", "BMW", "3 Series", 90.0); 

    rentalSystem.addCar(car1); 
    rentalSystem.addCar(car2); 
    rentalSystem.addCar(car3); 

    Customer* customer1 = new Customer("CUST001", "Mrityunjay" , "0823"); 
    Customer* customer2 = new Customer("CUST002", "Manas", "3948"); 

    rentalSystem.addCustomer(customer1); 
    rentalSystem.addCustomer(customer2); 

    rentalSystem.displayAvailableCars(); 

    std::string rental1 = rentalSystem.rentCar(car1, customer1, "2025-03-24", 3); 
    std::string rental2 = rentalSystem.rentCar(car2, customer2, "2025-10-11", 4); 

    if (!rental1.empty()){
        std::cout << "\nRental created successfully: " << rental1 << std::endl; 
    }

    if (!rental2.empty()){
        std::cout<< "\nRental create successfully: " << rental2 << std::endl; 
    }

    rentalSystem.displayRentals(); 

    rentalSystem.displayAvailableCars(); 

    if (rentalSystem.returnCar(rental1)){
        std::cout << "\nCar returned successfully for rental: " << rental1 << std::endl; 
    }

    rentalSystem.displayAvailableCars(); 

    return 0; 
}