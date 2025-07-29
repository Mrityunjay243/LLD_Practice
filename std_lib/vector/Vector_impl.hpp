#ifndef VECTOR_IMPL_HPP
#define VECTOR_IMPL_HPP

#include "Vector.hpp"
#include <algorithm>
#include <iostream>

template <typename T>
Vector<T>::Vector() : data(nullptr), sz(0), capacity(0){}

template <typename T>
Vector<T>::~Vector(){
    delete[] data; 
}

template <typename T>
Vector<T>::Vector(const Vector<T>& other) : sz(other.sz), capacity(other.capacity){
    data = new T[capacity]; 
    std::copy(other.data, other.data+sz, data); 
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other){
    if (this != other){
        delete[] data; 
        sz = other.sz; 
        capacity = other.capacity; 
        data = new T[capacity]; 
        std::copy(other.data, other.data+sz, data); 
    }

    return *this; 
}

template <typename T>
void Vector<T>::reallocate(size_t newCap){
    T* newData = new T[newCap]; 
    for (size_t i=0; i<sz; i++){
        newData[i] = data[i]; 
    }

    delete[] data; 
    data = newData; 
    capacity = newCap; 
}

template <typename T>
void Vector<T>::p_b(const T& value){
    if (sz == capacity){
        reallocate(capacity==0? 1 : capacity*2); 
    }
    data[sz++] = value; 
}

template <typename T>
void Vector<T>::pop(){
    if (sz > 0){
        sz--; 
    }
}

template <typename T>
T& Vector<T>::operator[](size_t index){
    if (index>=sz) throw std::out_of_range("Outa range dawg"); 
    return data[index]; 
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const{
    if (index>=sz) throw std::out_of_range("Outa range dawg"); 
    return data[index]; 
}

template <typename T>
size_t Vector<T>::size() const{
    return sz;
}

template <typename T>
size_t Vector<T>::cap() const {
    return capacity; 
}

template <typename T>
bool Vector<T>::empty() const {
    return sz==0; 
}

template <typename T>
void Vector<T>::reserve(size_t newCap){
    if (newCap>cap){
        reallocate(newCap);
    }
}

template <typename T>
void Vector<T>::resize(size_t newSize, const T& defaultValue){
    if (newSize > capacity){
        reallocate(newSize); 
    }

    for (size_t i=sz; i<newSize; i++){
        data[i] = defaultValue; 
    }

    sz = newSize; 
}

#endif