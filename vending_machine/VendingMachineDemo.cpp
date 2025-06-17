#include "VendingMachine.hpp"
#include <iostream>

int main() {
    VendingMachine machine("VM001");

    // Adding Products
    Product* cola = machine.addProduct("Cola", 2.50, 10);
    Product* chips = machine.addProduct("Chips", 1.50, 15);
    Product* candy = machine.addProduct("Candy", 1.00, 20);

    // Display the inital inventory 
    std::cout<< "Initial machine status: " << std::endl;
    machine.displayMachineInfo();
    machine.displayInventory();

    //making some purchases 
    std::cout<< "\n Making purchases..." << std::endl;
    Transaction* t1 = machine.purchaseProduct(cola->getProductId(), 2, 5.00);
    Transaction* t2 = machine.purchaseProduct(chips->getProductId(), 3, 5.00);

    // Displaying the transactions
    machine.displayTransactions();

    // Restock a product
    std::cout<< "\nRestocking Cola..." << std::endl;
    machine.updatePrice(candy->getProductId(), 1.25);

    // Display final status
    std::cout<< "\nFinal Machine status:"<<std::endl;
    machine.displayMachineInfo();
    machine.displayInventory();

    return 0;
}