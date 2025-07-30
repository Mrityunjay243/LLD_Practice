#ifndef LRU_HPP
#define LRU_HPP

#include <unordered_map>
#include <list>
#include <mutex>
#include <optional>

template <typename Key, typename Value>
class LRUCache{
    size_t capacity; 

    std::list<std::pair<Key, Value>> itemList; 

    std::unordered_map<Key, typename std::list<std::pair<Key, Value>>::iterator> itemMap; 

    mutable std::mutex mtx; 


    public:
        LRUCache(size_t capacity): capacity(capacity){}

        std::optional<Value> get(const Key& key){
            std::lock_guard<std::mutex> lock(mtx); 
            auto it = itemMap.find(key); 
            if (it == itemMap.end()){
                return std::nullopt; 
            }

            itemList.splice(itemList.begin(), itemList, it->second); 
            return it->second->second; 
        }

        void put(const Key& key, const Value& value){
            std::lock_guard<std::mutex> lock(mtx); 

            auto it = itemMap.find(key); 
            if (it != itemMap.end()){
                it->second->second = value; 
                itemList.splice(itemList.begin(), itemList, it->second); 
                return; 
            }

            if (itemList.size() == capacity){
                auto lru = itemList.back(); 
                itemMap.erase(lru.first); 
                itemList.pop_back(); 
            }

            itemList.emplace_front(key, value); 
            itemMap[key] = itemList.begin(); 
        }

        bool exists(const Key& key) const {
            std::lock_guard<std::mutex> lock(mtx); 
            return itemMap.find(key) != itemMap.end(); 
        }

        size_t size() const {
            std::lock_guard<std::mutex> lock(mtx); 
            return itemList.size(); 
        }

};

#endif