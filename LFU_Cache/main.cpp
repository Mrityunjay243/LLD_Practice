#include <iostream>
#include <bits/stdc++.h>
#include <unordered_map>

class Node{
    public:
        int key, value, count; 
        Node *prev, *next; 

        Node(int _key, int _value){
            key = _key; 
            value = _value; 
            count = 1; 
        }
}; 

class List {
    public:
        int size; 
        Node *head, *tail; 

        List(){
            head = new Node(0, 0); 
            tail = new Node(0, 0); 
            head->next = tail; 
            tail->prev = head; 
            size = 0; 
        }

        void addNode(Node *node){
            Node* temp = head->next; 
            head->next = node; 
            temp->prev = node; 
            node->next = temp; 
            node->prev = head; 
            size++; 
        }

        void deleteNode(Node *node){
            Node *prevN = node->prev; 
            Node *nextN = node->next; 
            prevN->next = nextN; 
            nextN->prev = prevN; 
            size--; 
        }
}; 

class LFUCache{
    unordered_map<int, List*> freqListMap; 
    unordered_map<int, Node*> keyNodeMap; 
    int maxi, minFreq, currSize; 

    public:
        LFUCache(int capacity){
            maxi = capacity; 
            minFreq = 0; 
            currSize = 0; 
        }

        int get(int key){
            if (keyNodeMap.find(key)!=keyNodeMap.end()){
                int res = keyNodeMap[key]->value; 
                updateFreqMap(keyNodeMap[key]); 
                return res; 
            }
            return -1; 
        }

        void updateFreqMap(Node *node){
            keyNodeMap.erase(node->key); 
            freqListMap[node->count]->deleteNode(node); 
            if (node->count == minFreq && freqListMap[node->count]->size==0){
                minFreq++; 
            }
            List *nextHigherFreqList = new List(); 
            if (freqListMap.find(node->count+1)!=freqListMap.end()){
                nextHigherFreqList = freqListMap[node->count+1]; 
            }

            node->count = node->count + 1; 
            nextHigherFreqList->addNode(node); 
            freqListMap[node->count] = nextHigherFreqList; 
            keyNodeMap[node->key] = node; 
        }

        void put(int key, int value){
            if (keyNodeMap.find(key)!=keyNodeMap.end()){
                keyNodeMap[key]->value = value; 
                updateFreqMap(keyNodeMap[key]); 
            } else {
                if (currSize == maxi){
                    List *ls = freqListMap[minFreq]; 
                    keyNodeMap.erase(ls->tail->prev->key); 
                    freqListMap[minFreq]->deleteNode(ls->tail->prev); 
                    currSize--; 
                }
                currSize++; 
                minFreq = 1; 
                List *listFreq = new List(); 
                if (freqListMap.find(minFreq)!=freqListMap.end()){
                    listFreq = freqListMap[minFreq]; 
                }
                Node *node = new Node(key, value); 
                listFreq->addNode(node); 
                keyNodeMap[key] = node; 
                freqListMap[minFreq] = listFreq; 
            }
        }
};