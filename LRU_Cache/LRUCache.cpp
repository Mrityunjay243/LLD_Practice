#include "LRUCache.hpp"


LRUCache::node::node(int key_, int value) : key(key_), val(value), next(nullptr), prev(nullptr) { }

LRUCache::LRUCache(int capacity){
    cap = capacity;
    head = new node(-1, -1);
    tail = new node(-1, -1);
    head->next = tail; 
    tail->prev = head; 
}

void LRUCache::addNode(node* newN){
    node* temp = head->next;
    newN->next = temp; 
    newN->prev = head; 
    head->next = newN; 
    temp->prev = newN; 
}

void LRUCache::deleteNode(node* delNode){
    node* prevN = delNode->prev; 
    node* nextN = delNode->next; 
    prevN->next = nextN; 
    nextN->prev = prevN; 
}



int LRUCache::get(int key){
    if (mp.find(key) != mp.end()){
        node *retN = mp[key]; 
        int res = retN->val; 
        mp.erase(key); 
        deleteNode(retN); 
        addNode(retN);
        mp[key] = head->next;
        return res;  
    }
    return -1; 
}

void LRUCache::put(int key, int value){
    if (mp.find(key) != mp.end()){
        node* existing = mp[key]; 
        mp.erase(key); 
        deleteNode(existing);
        delete existing;  
    }

    addNode(new node(key, value));
    mp[key] = head->next; 

    if (mp.size() > cap){
        node *temp = tail->prev; 
        mp.erase(temp->key); 
        deleteNode(temp); 
        delete temp; 
    }
}