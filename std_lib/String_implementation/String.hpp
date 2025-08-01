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
        String(String& other); 
        String(String&& other) noexcept; 
        String& operator=(const String& other); // copy assign
        String& operator=(String&& other); // move assign
        ~String(); 

        char& operator[](size_t index); 
        const char& operator[](size_t index) const;
        
        void push_back(char c); 
        String& operator+=(const String& other); 

        size_t size() const; 
        size_t length() const; 
        size_t cap() const; 
        const char* c_str() const; 
        void clear(); 

        bool operator==(const String& other) const; 
        friend std::ostream& operator<<(std::ostream& os, const String& s); 
}; 

#endif