**Requirements**
- The system should allow users to register and login to their accounts. 
- Users should be able to create new auction listings 
- Users should be able to browse and search auction listings 
- Users should be able to place bids on active auction listings 
- The system should automatically update the current highest bid, and notify the bidders accordingly 
- The auction should end when the specified duration is reached 

**Classes, Interfaces, and Enums**
- The User class represents a user in the online auction system 
- The AuctionStatus enum defines the possible states in the auction listing, such as active or closed
- The AuctionSystem class is the core of the online auction system, and follows the singleton pattern to ensure single instance of the auction system 
- The main class serves as the entry point to the program 
