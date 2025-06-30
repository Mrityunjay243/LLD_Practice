**Description**
- The ATM should support basic operations like balance enquiry, cash withdrawal, and cash deposit 
- Users should be able to authenticate themselves using a card and a PIN 
- The system should interact with a bank's backend to validate user accounts and perform transactions 
- The ATM should have a cash dispenser to dispense cash to the users 
- The system should be able to handle concurrent requests and ensure data consistency 


**Classes, Interfaces, and Enums**
- The CARD Class represents the ATM card with a card number and pin 
- The account Class represents a bank account with an account number and balance
- The transactions class is an abstract class for different kinds of transactions, it should be extended by withdrawTransaction and depositTransaction 
- The BankingService class manages bank accounts and processes transactions, it uses a thread safe concurrent hashmap to store and retrive account info 
- The CashDispenser class represents the ATM's cash dispenser, and handles the dispensing of cash. It uses synchronization to ensure thread safety when dispensing cash 
- The ATM class is the main interface for ATM operations, it interacts with BankingService and CashDispenser to perform user authentication, Balance Enquiry, cash withdrawal, and cash deposit 
- The ATMDriver class demonstrates the usage of ATM system by creating sample accounts and performing ATM operations 