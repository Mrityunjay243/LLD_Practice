#include "ParkingLot.hpp"
#include <iostream>

int main(){
    // Creating parking lot with different types of spots 
    ParkingLot parkingLot(2, 3, 2);

    std::cout<< "Initial Parking Lot Status: "<< std::endl; 
    parkingLot.displayInfo();

    // Creating Vehicles
    Vehicle *car1 = new Vehicle("CAR001", VehicleType::CAR, "Red");
    Vehicle *car2 = new Vehicle("CAR002", VehicleType::CAR, "Blue");
    Vehicle *motorcycle = new Vehicle("MOTO001", VehicleType::MOTORCYCLE, "Black");
    Vehicle *truck = new Vehicle("TRUCK001", VehicleType::TRUCK, "White");

    // Parking Vehicles 
    std::cout<< "\nParking Vehicles..."<< std::endl; 

    if (parkingLot.parkVehicle(car1)){
        std::cout<<"Parked Successfully"; 
        car1->displayInfo();
    }

    if (parkingLot.parkVehicle(motorcycle)) {
        std::cout << "Parked successfully: "; 
        truck->displayInfo();
    }

    if (parkingLot.parkVehicle(truck)){
        std::cout<<"Parked Successfully: "; 
        truck->displayInfo();
    }

    //std::cout<<"Checking control";

    // Display current status 
    parkingLot.displayInfo();
    parkingLot.displayOccupancy();

    std::cout<< "\nRemoving vehicle CAR001..." << std::endl;
    Vehicle* removed = parkingLot.removeVehicle("CAR001");
    if (removed){
        std::cout<<"Removed Successfully: ";
        removed->displayInfo();
        delete removed; 
    }

    // Try parking another car
    if (parkingLot.parkVehicle(car2)){
        std::cout<<"Parked Successfully";
        car2->displayInfo();
    }

    // Final Status
    std::cout<<"\nFinal Parking Lot Status: "<<std::endl;
    parkingLot.displayInfo();
    parkingLot.displayOccupancy();

    // cleaning up vehicles 
    delete car2; 
    delete motorcycle; 
    delete truck; 

    return 0;
}