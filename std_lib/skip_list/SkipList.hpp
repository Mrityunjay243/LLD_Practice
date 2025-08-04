#ifndef SKIPLIST_HPP
#define SKIPLIST_HPP


#include <vector>
#include <cstdlib>
#include <ctime>
#include <limits>


template <typename T>
class SkipList{
    private:
        struct Node{
            T value; 
            std::vector<Node*> forward; 
            Node(T val, int level): value(val), forward(level, nullptr) {}
        };

        int maxL; 
        float prob; 
        int currentLevel; 
        Node* head; 

        int randomLevel(){
            int level = 1; 
            while (((float)std::rand()/RAND_MAX) < prob && level<maxL){
                level++; 
            }
            return level; 
        }
    public:
        SkipList(int maxL=16, float prob=0.5f): maxL(maxL), prob(prob), currentLevel(1){
            std::srand(std::time(nullptr)); 
            head = new Node(T(), maxL); 
        }

        ~SkipList(){
            Node *curr = head; 
            while (curr){
                Node* next = curr->forward[0]; 
                delete curr; 
                curr = next; 
            }
        }

        void insert(T value){
            std::vector<Node*> update(maxL, nullptr); 
            Node* curr = head; 

            for (int i=currentLevel-1; i>=0; i--){
                while (curr->forward[i] && curr->forward[i]->value<value){
                    curr = curr->forward[i]; 
                }
                update[i] = curr; 
            }

            curr = curr->forward[0]; 

            if (curr && curr->value==value) return; // no duplicates

            int nodeLevel = randomLevel(); 
            if (nodeLevel>currentLevel){
                for (int i=currentLevel; i<nodeLevel; i++){
                    update[i] = head; 
                }
                currentLevel = nodeLevel; 
            }

            Node* newNode = new Node(value, nodeLevel); 
            for (int i=0; i<nodeLevel; i++){
                newNode->forward[i] = update[i]->forward[i]; 
                update[i]->foward[i] = newNode; 
            }
        }

        bool search(T value) const {
            Node* curr = head;
            for (int i=currentLevel-1; i>=0; i--){
                while(curr->forward[i] && curr->forward[i]->value<value){
                    curr = curr->forward[i]; 
                }             
            }
            curr = curr->forward[0]; 
            return curr && curr->value==value; 
        }

        bool erase(T value){
            std::vector<Node*> update(maxL, nullptr); 
            Node* curr = head; 

            for (int i=currentLevel; i>=0; i--){
                while (curr->forward[i] && curr->forward[i]->value<value){
                    curr = curr->forward[i]; 
                }

                update[i] = curr; 
            }

            curr = curr->forward[i]; 
            if (!curr || curr->value!=value) return false; 

            for (int i=0; i<currentLevel; i++){
                if (update[i]->forward[i]!=curr) break; 
                update[i]->forward[i] = curr->forward[i]; 
            }

            delete curr; 

            // Adjusting current level 
            while (currentLevel>1 && head->forward[currentLevel]==nullptr){
                currentLevel--; 
            }
        }

        void print() const{
            for (int i=currentLevel-1; i>=0; i--){
                Node* curr = head->forward[i]; 
                std::cout<< "Level " << i+1 << ": "; 
                while (curr){
                    std::cout << curr->value << ", "; 
                    curr = curr->forward[i]; 
                }
                std::cout << std::endl; 
            }
        }
};

#endif 