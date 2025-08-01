#include "String.hpp"

String::String(): data(new char[1]{'\0'}), len(0), capacity(1) {}

String::String(const char* str){
    len = std::strlen(str); 
    capacity = len+1; 
    data = new char[capacity];
    std::strcpy(data, str); 
}

String::String(String& other): len(other.len), capacity(other.capacity){
    data = new char[capacity]; 
    std::strcpy(data, other.data); 
}

String::String(String&& other) noexcept: data(other.data), len(other.len), capacity(other.capacity){
    other.data = nullptr; 
    other.len = 0; 
    other.capacity = 0; 
}

String& String::operator=(const String& other){
    if (this!=&other){
        delete[] data; 
        len = other.len; 
        capacity = other.capacity; 
        data = new char[capacity]; 
        std::strcpy(data, other.data); 
    }

    return *this; 
}

String& String::operator=(String&& other) {
    if (this!=&other){
        delete[] data; 
        data = other.data; 
        len = other.len; 
        capacity = other.capacity; 
        other.data = nullptr; 
        other.len = 0; 
        other.capacity = 0; 
    }

    return *this; 
}

String::~String(){
    delete[] data; 
}

char& String::operator[](size_t index){
    return data[index]; 
}

const char& String::operator[](size_t index) const{
    return data[index]; 
}

void String::reallocate(size_t newCap){
    char* newData = new char[newCap]; 
    std::strcpy(newData, data); 
    delete[] data; 
    data = newData; 
    capacity = newCap; 
} 

void String::push_back(char c){
    if (len+1>capacity){
        reallocate(capacity*2); 
    }

    data[len++] = c; 
    data[len] = '\0'; 
}

String& String::operator+=(const String& other){
    while (len+other.len+1>capacity){
        reallocate(capacity*2); 
    }
    std::strcat(data, other.data); 
    len += other.len; 
    return *this; 
}

size_t String::size() const {
    return len; 
}

size_t String::length() const {
    return len; 
}

size_t String::cap() const {
    return capacity; 
}

const char* String::c_str() const{
    return data; 
}

void String::clear(){
    len = 0; 
    data[0] = '\0'; 
}

bool String::operator==(const String& other) const {
    return std::strcmp(data, other.data) == 0;
}

std::ostream& operator<<(std::ostream& os, const String& s) {
    os << s.data; 
    return os; 
}