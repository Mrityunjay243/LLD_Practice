#ifndef OBJECT_POOL_HPP
#define OBJECT_POOL_HPP

#include <concepts>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <new>

namespace mijun{
    
    template <typename ObjectT>
    class ObjectPool{
        /*
        A node is a single slot in the pool. 
        We don't default construct objectT here - we just reserve raw. 
        suitably aligned storage and use placement when the user calls alloc(...). 
        */
       struct Node final{
            alignas(ObjectT) std::byte storage[sizeof(ObjectT)]; 
            Node* next{}; 
            [[nodiscard]] ObjectT* obj() noexcept{
                return std::launder(reinterpret_cast<ObjectT*>(storage)); 
            }
       }; 

       // Each dynamic allocation grabs a whole batch of nodes at once
       struct Batch final{
            std::size_t size{}; 
            Node* nodes{}; 
            Batch* next{}; 
       }; 

       Batch* batches_; 
       Node* freeList_; 
       std::size_t capacity_; 
       std::size_t allocated_;

       public:
            /*
            Move only constructor assignment
            */

            ObjectPool() noexcept : ObjectPool(1) {}

            explicit ObjectPool(std::size_t preallocations) noexcept: 
                    batches_{nullptr}, 
                    freeList_{nullptr}, 
                    capacity_{0}, 
                    allocated_{0}{
                        initialize(preallocations); 
                    }

            
            // move constructor
            ObjectPool(ObjectPool&& other) noexcept:
                    batches_{std::exchange(other.batches_, nullptr)}, 
                    freeList_{std::exchange(other.freeList_, nullptr)}, 
                    capacity_{std::exchange(other.capacity_, 0)}, 
                    allocated_{std::exchange(other.allocated_, 0)}
                    {}
            
            // move assign
            ObjectPool& operator=(ObjectPool&& other) noexcept{
                if (this!=&other){
                    this->~ObjectPool(); // clean up the current resources
                    
                    new (this) ObjectPool(std::move(other)); 
                }

                return *this; 
            }

            ObjectPool(const ObjectPool&)  = delete; 
            ObjectPool& operator=(const ObjectPool&) = delete; 

            /*
            Public API
            */

            template<typename... ArgTs>
            ObjectT& alloc(ArgTs&&... args) noexcept(std::is_nothrow_constructible_v<ObjectT, ArgTs...>)
            {
                if (freeList_ == nullptr) grow(); // we need more nodes 

                Node* node = std::exchange(freeList_, freeList_->next); 
                ++allocated_; 
                std::construct_at(node->obj(), std::forward<ArgTs>(args)...); 
                return *node->obj(); 
            }

            void free(ObjectT& object) noexcept(
                std::is_nothrow_destructible_v<ObjectT>){
                    std::destroy_at(&object); 

                    Node* node = reinterpret_cast<Node*>(&object); 
                    node->next = freeList_; 
                    freeList_ = node; 
                    --allocated_; 
            }

            [[nodiscard]] std::size_t numAllocated() const noexcept{
                return allocated_; 
            }

            [[nodiscard]] std::size_t capacity() const noexcept{
                return capacity_; 
            }

            /*
            Destructor
            */
           ~ObjectPool() noexcept{
                for (Batch* b = batches_; b!=nullptr;){
                    Batch* nxt = b->next; 
                    delete[] b->nodes; 
                    delete b; 
                    b = nxt;
                }
           }
        private:
           void initialize(std::size_t prealloc) noexcept{
                allocateBatch(prealloc); 
                capacity_ = prealloc; 
           }

           void grow(){
                allocateBatch(capacity_); 
                capacity_ <<= 1; 
           }

           void allocateBatch(std::size_t n){
                auto* batch = new Batch{n, new Node[n], batches_}; 
                batches_ = batch; 

                // push every fresh node into the free list
                for (std::size_t i=0; i<n; i++){
                    batch->nodes[i].next = freeList_; 
                    freeList_ = &batch->nodes[i]; 
                }
           }
    };
} // end of namespace


#endif