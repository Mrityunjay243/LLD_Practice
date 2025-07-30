#ifndef STRING_HPP
#define STRING_HPP

#include <cstring>
#include <iostream>

class String {
    private:
        char* data; 
        size_t len; 
        size_t capacity;

        void reallocate(size_t newCap); 
        
    public:

        String(); 
        String(const char* str); 
        String(String& other) noexcept; 
        String(String&& other) noexcept; 
        String& operator=(const String& other); // copy assign
        String& operator=(const String&& other); // move assign
        ~String(); 

        char& operator[](size_t index); 
        const char& operator[](size_t index) const;
        
        void push_back(char c); 
        String& operator+=(const String& other); 

        size_t size(); 
        size_t length(); 
        size_t capacity(); 
        const char* c_str(); 
        void clear(); 

        bool operator==(const String& other) const; 
        friend std::ostream& operator<<(std::ostream& os, const String& s); 
}; 

#endif