#ifndef SHARED_PTR_HPP
#define SHARED_PTR_HPP

#include <cstddef> 
#include <utility>

template<typename T>
class SharedPtr{
    private:
        T* ptr; 
        std::size_t* count; 

        void release(){
            if (count){
                (*count)--; 
                if (*count==0){
                    delete ptr; 
                    delete count; 
                }
            }
        }

    public:
        SharedPtr(): ptr(nullptr), count(nullptr) {}

        // constructor from a raw pointer
        explicit SharedPtr(T* p): ptr(p), count(new std::size_t(1)) {} 

        // Copy constructor
        SharedPtr(SharedPtr<T>& other) noexcept : ptr(other), count(other.count){
            if (count) ++(*count); 
        }

        // Move constructor
        SharedPtr(SharedPtr<T>&& other) noexcept: ptr(other.ptr), count(other.count){
            other.ptr = nullptr; 
            other.count = nullptr; 
        }

        // Copy assignment
        SharedPtr& operator=(const SharedPtr<T>& other){
            if (this != &other){
                release(); 
                ptr = other.ptr; 
                count = other.count; 
                if (count) ++(*count); 
            }
        }

        SharedPtr& operator=(SharedPtr<T>&& other) noexcept{
            if (this!=other){
                release(); 

                ptr = other.ptr; 
                count = other.count; 
                other.ptr = nullptr; 
                other.count = nullptr; 
            }

            return *this; 
        }

        ~SharedPtr(){
            release(); 
        }

        T& operator*() const {
            return *ptr; 
        }

        T& operator->() const {
            return ptr; 
        }

        std::size_t use_count() const {
            return count? *count : 0; 
        }

        T* get() const {
            return ptr; 
        }

        void reset(T* p = nullptr){
            release(); 
            ptr = p; 
            count = p ? new std::size_t(1) : nullptr; 
        } 
}; 

#endif