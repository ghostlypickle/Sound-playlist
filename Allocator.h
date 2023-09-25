#ifndef UNTITLED12_ALLOCATOR_H
#define UNTITLED12_ALLOCATOR_H
#include <iostream>
#include <limits>

template<class T>
class Allocator{
public:
    Allocator() = default;
    ~Allocator() = default;

    template <typename U>
    struct rebind {
        using other = Allocator<U>;
    };

    T* allocate(size_t number_of_objects){
        if(number_of_objects <= 0){
            throw std::bad_alloc();
        }
        if(number_of_objects > getMaxAllocations()){
            throw std::bad_alloc();
        }

        return new T[number_of_objects];
    }

    template <typename... Args>
    T* construct(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

    void deallocate(T* pointer){
        if(pointer == nullptr){
            return;
        }
        delete [] pointer;
    }

    void destroy(T* pointer){
        if(pointer == nullptr){
            return;
        }
        pointer->~T();
    }

private:
    size_t getMaxAllocations(){
        return std::numeric_limits<size_t>::max() / sizeof(T);
    }
};

#endif //UNTITLED12_ALLOCATOR_H
