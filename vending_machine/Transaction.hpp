#ifndef Transaction_hpp
#define Transaction_hpp

#include <string>
#include <ctime>

class Transaction{
    private: 
        std::string transactionId;
        std::string productId; 
        int quantity; 
        double amount; 
        std::time_t timestamp;
        bool successful;

    public: 
        Transaction(std::string transactionId, std::string productId, int quantity, double amount);
        

        std::string getTransactionId() const;
        std::string getProductId() const;
        int getQuantity() const; 
        double getAmount() const;
        std::time_t getTimestamp() const;
        bool isSuccessful() const;

        void setSuccessful(bool status);
        void displayInfo() const;
};

#endif