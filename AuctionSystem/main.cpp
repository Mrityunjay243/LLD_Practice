#include "AuctionSystem.hpp"
#include <iostream>
#include <ctime>

int main(){

    AuctionSystem system; 

    User* seller = system.registerUser("Mj", "mj@gmail.com"); 
    User* buyer1 = system.registerUser("B1", "B1@gamil.com"); 
    User* buyer2 = system.registerUser("B2", "B2@gamil.com"); 

    system.addUserBalance(buyer1->getUserId(), 100.0); 
    system.addUserBalance(buyer2->getUserId(), 150.0); 

    std::cout<<" Initial user balance: " << std::endl; 
    system.displayUserInfo(buyer1->getUserId()); 
    system.displayUserInfo(buyer2->getUserId()); 

    Item *phone = system.createItem(seller->getUserId(), "Smart Phone", "Last Model SMP", ItemCategory::ELECTONICS, 50.0);
    Item *watch = system.createItem(seller->getUserId(), "Luxury Watch", "Rolex limited edition", ItemCategory::FASHION, 100.0); 
    
    if (phone && watch){
        std::cout << "\nCreated items: "<< std::endl; 
        system.displayItemInfo(phone->getItemId()); 
        system.displayItemInfo(watch->getItemId());

        std::time_t now = std::time(nullptr);
        std::time_t oneHourLater = now + 3600;

        Auction* phoneAuction = system.createAuction(phone->getItemId(), now, oneHourLater); 
        Auction* watchAuction = system.createAuction(watch->getItemId(), now, oneHourLater); 

        if (phoneAuction && watchAuction){
            std::cout<<"Action Auctions: " << std::endl; 
            system.displayAllAuctions(); 

            std::cout<< "\nPlacing bids: " << std::endl; 
            if (system.placeBid(phoneAuction->getAuctionId(), buyer1->getUserId(), 60.0)){
                std::cout<<"Bid placed successfully by " << buyer1->getUsername() << std::endl; 
            }
            if (system.placeBid(phoneAuction->getAuctionId(), buyer2->getUserId(), 70.0)){
                std::cout<<"Bid placed successfully by " << buyer1->getUsername() << std::endl; 
            }
            if (system.placeBid(watchAuction->getAuctionId(), buyer2->getUserId(), 120.0)){
                std::cout<<"Bid placed successfully by " << buyer2->getUsername() << std::endl; 
            }

            std::cout << "\nCurrent Auction Status: " << std::endl; 
            system.displayAuctionInfo(phoneAuction->getAuctionId()); 
            system.displayAuctionInfo(watchAuction->getAuctionId()); 

            std::cout<< "\nEnding Auctions: "<< std::endl; 
            system.endAuction(phoneAuction->getAuctionId()); 
            system.endAuction(watchAuction->getAuctionId()); 

            std::cout<< "\nFinal Auction Results: " << std::endl; 
            system.displayAuctionInfo(phoneAuction->getAuctionId()); 
            system.displayAuctionInfo(watchAuction->getAuctionId()); 


            std::cout<<"\nFinal User Balances: " << std::endl; 
            system.displayUserInfo(buyer1->getUserId()); 
            system.displayUserInfo(buyer2->getUserId()); 
        }
    }

    return 0; 
}