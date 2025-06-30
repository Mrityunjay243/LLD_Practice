#include "Task.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

Task::Task(std::string taskId, std::string title, std::string description, TaskPriority priority)
    : taskId(taskId), title(title), description(description), status(TaskStatus::TODO), priority(priority) {
        dueDate = 0; 
    }

std::string Task::getTaskId() const { return taskId; }
std::string Task::getTitle() const { return title; }
std::string Task::getDescription() const { return description; }
std::string Task::getAssignedTo() const { return assignedTo; }
TaskStatus Task::getStatus() const { return status; }
TaskPriority Task::getPriority() const { return priority; }
std::time_t Task::getDueDate() const { return dueDate; }
const std::vector<std::string>& Task::getDependencies() const { return dependencies; }
const std::vector<std::string>& Task::getComments() const { return comments; }

void Task::setAssingedTo(const std::string& userId){
    assignedTo = userId; 
}

void Task::setStatus(TaskStatus status){
    this->status = status; 
}

void Task::setPriority(TaskPriority priority){
    this->priority = priority; 
}

void Task::setDueDate(std::time_t dueDate){
    this->dueDate = dueDate;
}

void Task::addDependency(const std::string& taskId){
    if(std::find(dependencies.begin(), dependencies.end(), taskId) == dependencies.end()){
        dependencies.push_back(taskId); 
    }
}

void Task::addComment(const std::string& comment){
    auto it = std::find(dependencies.begin(), dependencies.end(), taskId); 
    if (it != dependencies.end()){
        dependencies.erase(it); 
    }
}

void Task::addComment(const std::string& comment){
    comments.push_back(comment); 
}

void Task::displayInfo() const {
    std::cout<< "\nTask: " << title << " (ID: " << taskId << ")" << std::endl; 
    std::cout<< "Description: " << description << std::endl; 
    std::cout<< "Assigned to: " << (assignedTo.empty() ? "Unassigned" : assignedTo) << std::endl; 

    std::cout<<"Status: "; 
    switch (status){
        case TaskStatus::TODO: std::cout<< "Low"; break; 
        case TaskStatus::IN_PROGRESS: std::cout<< "In progress"; break;
        case TaskStatus::COMPLETED: std::cout<< "Medium"; break;
        case TaskStatus::BLOCKED: std::cout<< "Medium"; break;
    }

    std::cout<< std::endl; 

    if (dueDate!=0){
        std::cout<<"Due date: " << std::ctime(&dueDate); 
    }

    if (!dependencies.empty()){
        std::cout<< "Dependencies: "; 
        for (const auto& dep: dependencies){
            std::cout<< dep << " "; 
        }
        std::cout << std::endl; 
    }

    if (!comments.empty()){
        std::cout<< "Comments: "<< std::endl; 
        for (const auto& comment: comments){
            std::cout<< "- " << comment << std::endl; 
        }
    }
}