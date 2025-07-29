#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>

template<typename T>
class Vector{
    private:
        T* data; 
        size_t sz; 
        size_t capacity; 

        void reallocate(size_t newCap); 

    public:
        Vector(); 
        ~Vector(); 

        Vector(const Vector& other); // copy constructor
        Vector& operator=(const Vector& other); // copy operator

        void p_b(const T& value); 
        void pop(); 

        T& operator[](size_t index); 
        const T& operator[](size_t index) const; 

        size_t size() const; 
        size_t capacity() const; 
        bool empty() const; 

        void reserve(size_t newCap); 
        void resize(size_t newSize, const T& defaultValue= T()); 
};

#include "Vector.cpp"

#endif
