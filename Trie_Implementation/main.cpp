#include <iostream>
using namespace std; 

// Node Structure of the Trie
struct Node {
    // Each index represents a letter 
    // Array to store link to child nodes 
    Node* links[26];
    // flag indicating if the node marks the end of a word 
    bool flag = false; 

    // check if the node contains a specific key or not (letter)
    bool containsKey(char ch){
        return links[ch - 'a'] != NULL;
    }

    // Insert a new node with a specific letter as the key 
    void put(char ch, Node* node){
        links[ch-'a'] = node; 
    }

    Node* get(char ch){
        return links[ch-'a'];
    }

    void setEnd(){
        flag = true; 
    }

    bool isEnd(){
        return flag; 
    }
};

class Trie {
    private:
        Node* root; 

    public: 
        Trie(){
            root = new Node();
        }

        void insert(string word){
            Node* node = root; 
            for (int i=0; i<word.size(); i++){
                if (!node->containsKey(word[i])){
                    node->put(word[i], new Node());
                }

                node = node->get(word[i]);
            }
            node->setEnd();
        }

        bool search(string word){
            Node* node = root; 
            for (int i=0; i<word.length(); i++){
                if (!node->containsKey(word[i])) return false; 

                node = node->get(word[i]);
            }

            return node->isEnd();
        }

        bool startsWith(string prefix){
            Node* node = root; 
            for (int i=0; i<prefix.size(); i++){
                if (!node->containsKey(prefix[i])) return false; 
                node = node->get(prefix[i]);
            }
            return true;
        }
};

int main(){
    Trie trie; 
    cout << "Inserting words: abc, abcdef, abcxyz, abcliqor" << endl; 
    trie.insert("abc");
    trie.insert("abcdef");
    trie.insert("abcxyz");
    trie.insert("abcliqor");

    cout<<"Check if abcmjf exists: "<< (trie.search("abcmjf") ? "True" : "False") << endl; 
    cout<<"Check if abcdef exists: "<< (trie.search("abcdef") ? "True" : "False") << endl;
    cout<<"Check if there are words staring with abcd: "<< (trie.startsWith("abcd") ? "True" : "False") << endl;  

    return 0; 

}