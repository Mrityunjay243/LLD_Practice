#ifndef STACK_OVERFLOW_HPP
#define STACK_OVERFLOW_HPP

#include <vector>
#include <string>
#include "User.hpp"
#include "Post.hpp"
#include "Comment.hpp"

class StackOverflow{

    private: 
        std::vector<User* > users; 
        std::vector<Post* > posts;
        int userIdCounter; 
        int postIdCounter; 
        int commentIdCounter; 
    
    public: 
        StackOverflow();
        ~StackOverflow();

        // User Management 
        User* registerUser(const std::string& userName, const std::string& email);
        void removeUser(const std::string& userId);

        // Post Managemet 
        Post* addQuestion(const std::string& userId, const std::string& content,
                        const std::vector<std::string>& tags);
        
        Post* addAnswer(const std::string& userId, const std::string& postId, 
                        const std::string& content);
        
        Comment* addComment(const std::string& userId, const std::string& postId, 
                        const std::string& content);

        // voting and Acceptance
        bool votePost(const std::string& userId, const std::string& postId);
        bool unvotePost(const std::string& userId, const std::string& postId);
        bool acceptAnswer(const std::string& userId, const std::string& answerId);

        // search and display
        std::vector<Post* > searchQuestions(const std::string& tag) const; 
        void displayUserProfile(const std::string& userId) const; 
        void displayQuestion(const std::string& questionId) const; 
        void displayAllQuestions() const; 

    private: 
            User* findUser(const std::string& userId) const; 
            Post* findPost(const std::string& postId) const; 
            void updateUserReputation(const std::string& userId, int points);
            std::string generateUserId();
            std::string generatePostId(); 
            std::string generateCommentId();

};

#endif