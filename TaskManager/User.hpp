#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <string>
#include <vector>

class User {
    private:
        std::string userId; 
        std::string username; 
        std::string email; 
        std::vector<std::string> assingedTasks; 
        bool active; 

    public: 
        User(std::string userId, std::string username, std::string email); 

        std::string getUserId() const; 
        std::string getUserName() const; 
        std::string getEmail() const; 
        const std::vector<std::string> getAssignedTasks() const; 
        bool isActive() const; 

        void addTask(const std::string& taskId); 
        void removeTask(const std::string& taskId); 
        void setActive(bool status); 
        void displayInfo() const; 
};

#endif