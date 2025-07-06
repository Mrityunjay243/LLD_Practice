#include "Car.hpp"
#include <iostream>
#include <iomanip>
 
Car::Car(std::string carId, std::string brand, std::string model, double basePrice):
        carId(carId), brand(brand), model(model), basePrice(basePrice), available(true) {}

std::string Car::getCarId() const { return carId; }
std::string Car::getCarBrand() const { return brand; }
std::string Car::getCarModel() const { return model; }
double Car::getBasePrice() const { return basePrice; }
bool Car::isAvailable() const  { return available; }

void Car::setAvailable(bool status){
    available = status; 
}

void Car::displayInfo() const {
    std::cout<< "Car Id: " << carId << std::endl; 
    std::cout<< "Brand: " << brand << std::endl; 
    std::cout<< "Model: " << model << std::endl; 
    std::cout<< "Base price per day: $" << std::fixed << std::setprecision(2) << basePrice << std::endl; 
    std::cout<< "Status: " << (available ? "Available" : "Rented") << std::endl; 
}