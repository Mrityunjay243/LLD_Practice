#ifndef LRUCACHE_HPP
#define LRUCACHE_HPP

#include <unordered_map>

class LRUCache{
    public:
        LRUCache(int capacity); 
        int get(int key); 
        void put(int key, int value);
        class node {
            public: 
                int key; 
                int val; 
                node* next; 
                node* prev; 
                node(int key_, int value); 
        };

        node* head; 
        node* tail; 
        
    private: 
        int cap; 
        std::unordered_map<int, node*> mp; 

        void addNode(node* newNode); 
        void deleteNode(node* delNode); 
}; 

#endif