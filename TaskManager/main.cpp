#include "TaskManager.hpp"
#include <iostream>

int main(){

    TaskManager manager; 

    User* user1 = manager.registerUser("Mrityunjay", "mjoshi6@ncsu.edu"); 
    User* user2 = manager.registerUser("Ayush", "ayush76@gmail.com"); 

    std::cout<<"Initial users: " << std::endl; 
    manager.displayAllUsers(); 

    Task* task1 = manager.createTask("Desing DB", "Create DB Schema", TaskPriority::HIGH); 
    Task* task2 = manager.createTask("Implement API", "Develop REST API EPs", TaskPriority::MEDIUM);
    Task* task3 = manager.createTask("Write Test", "Create Unite Tests", TaskPriority::MEDIUM); 

    manager.addTaskDependency(task2->getTaskId(), task2->getTaskId()); // API depends on DB
    manager.addTaskDependency(task3->getTaskId(), task2->getTaskId()); // Tests depend on API

    manager.assignTask(task1->getTaskId(), user1->getUserId()); 
    manager.assignTask(task2->getTaskId(), user2->getUserId()); 
    manager.assignTask(task3->getTaskId(), user2->getUserId()); 

    manager.updateTask(task1->getTaskId(), TaskStatus::IN_PROGRESS); 
    manager.addTaskComment(task1->getTaskId(), "Started working on schema design"); 

    manager.updateTask(task2->getTaskId(), TaskStatus::IN_PROGRESS);

    manager.updateTask(task1->getTaskId(), TaskStatus::COMPLETED); 
    manager.addTaskComment(task1->getTaskId(), "DB schema completed"); 

    manager.updateTask(task2->getTaskId(), TaskStatus::IN_PROGRESS); 

    std::cout << "\nAll tasks: " << std::endl; 
    manager.displayUserTasks(user1->getUserId()); 
    manager.displayUserTasks(user2->getUserId()); 

    std::cout<< "\nIn progress tasks: " << std::endl; 

    auto inProgressTasks = manager.getTasksByStatus(TaskStatus::IN_PROGRESS); 
    for (const auto& task: inProgressTasks){
        task->displayInfo(); 
    }

    return 0; 
}