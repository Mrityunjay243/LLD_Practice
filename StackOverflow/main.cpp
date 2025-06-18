#include "StackOverflow.hpp"
#include <iostream>

int main(){

    StackOverflow stackoverflow; 

    // Register Users 
    User* user1 = stackoverflow.registerUser("Mijun", "mijun@gmail.com");
    User* user2 = stackoverflow.registerUser("Divit", "divit@gmail.com");
    User* user3 = stackoverflow.registerUser("Rushil", "rushil@gmail.com");

    std::cout<< "Initial Users: " << std::endl; 
    stackoverflow.displayUserProfiles(user1->getUserId());

    // Adding questions 
    std::vector<std::string> tags = {"C++", "programming"};
    Post* question1 = stackoverflow.addQuestion(
        user1->getUserId(),  
        "How do I use smart pointer in Cpp", 
        tags
    ); 
    
    // Adding answers
    Post* answer1 = stackoverflow.addAnswer(
        user2->getUserId(), 
        question1->getPostId(), 
        "Smart Pointers automatically manage memory for you..."
    ); 

    Post *answer2 = stackoverflow.addAnswer(
        user3->getUserId(), 
        question1->getPostId(), 
        "There are three main types of smart pointers"
    ); 

    // adding comments 
    stackoverflow.addComment(
        user1->getUserId(), 
        answer1->getPostId(), 
        "Thanks, that's helpful!"
    ); 

    // Voting on posts
    stackoverflow.votePost(user2->getUserId(), question1->getPostId());
    stackoverflow.votePost(user3->getUserId(), answer1->getPostId());
    stackoverflow.votePost(user1->getUserId(), answer2->getPostId());

    // Accept answer 
    stackoverflow.acceptAnswer(user1->getUserId(), answer1->getPostId());

    // Display Results
    std::cout<< "\nQuestion with answers: "<< std::endl;
    stackoverflow.displayQuestion(question1->getPostId());

    std::cout<< "\nUser Profiles after activity: "<< std::endl;
    stackoverflow.displayUserProfiles(user1->getUserId());
    stackoverflow.displayUserProfiles(user2->getUserId());

    // Search Questions 
    std::cout<< "\nSearching for C++ questions" << std::endl;
    auto results = stackoverflow.searchQuestions("C++");
    for (const auto& question: results){
        question->displayInfo();
    }


    return 0;
}