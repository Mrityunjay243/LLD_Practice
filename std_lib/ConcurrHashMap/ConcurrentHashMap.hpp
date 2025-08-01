#ifndef CONCURRENT_HASHMAP_HPP
#define CONCURRENT_HASHMAP_HPP

#include <vector>
#include <list>
#include <mutex>
#include <shared_mutex>
#include <optional>
#include <functional>

template <typename K, typename V, size_t NumBuckets = 19>
class ConcurrentHashMap{
    private:
        struct Bucket{
            std::list<std::pair<K, V>> data; 
            mutable st::mutex mtx; 
        };

        std::vector<Bucket> buckets; 

        Bucket& getBucket(const K& key){
            size_t hash = std::hash<K>{}(key); 
            return buckets[hash%NumBuckets]; 
        }
        
    public:
        ConcurrentHashMap(): buckets(NumBuckets){}

        void insert(const K& key, const V& value){
            Bucket& bucket = getBucket(key); 
            std::lock_guard<std::mutex> lock(bucket.mtx); 

            for (auto& [k, v]: bucket.data){
                if (k == key){
                    v = value; 
                    return; 
                }
            }

            bucket.data.emplace_back(key, value); 
        }

        std::optional<V> get(const K& key){
            Bucket& bucket = get_bucket(key); 
            std::lock_guard<std::mutex> lock(bucket.mtx);
            
            for (const auto& [k, v]: bucket){
                if (k==key){
                    return v; 
                }
            }

            return std::nullopt; 
        }

        void remove(const K& key){
            Bucket& bucket = getBucket(key); 
            std::lock_guard<std::mutex> lock(bucket.mtx); 

            bucket.data.remove_if([&](const std::pair<K, V>& p){
                return p.first = key; 
            }); 
        }

        bool contains(const K& key) const{
            return get(key).has_value(); 
        }
};

#endif