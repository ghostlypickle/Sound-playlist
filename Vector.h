#ifndef UNTITLED12_VECTOR_H
#define UNTITLED12_VECTOR_H
#include "Allocator.h"
#include <iostream>

template<class T>
class Vector{
private:
    T* m_data;
    size_t m_capacity;
    size_t m_length;
    Allocator<T> allocator;

public:
    Vector() : m_capacity(4), m_length(0){
        m_data = allocator.allocate(m_capacity);
    }

    Vector(const Vector<T> &other) : m_capacity(other.m_capacity), m_length(other.m_length){
        allocator.deallocate(m_data);
        m_data = allocator.allocate(m_capacity);
        for(size_t i = 0; i < m_length; ++i){
            m_data[i] = other.m_data[i];
        }
    }

    Vector(Vector<T> &&other) : m_data(other.m_data), m_capacity(other.m_capacity), m_length(other.m_length){
        other.m_data = nullptr;
        other.m_capacity = 0;
        other.m_length = 0;
    }

    ~Vector(){
        clear();
    }

    void clear(){
        allocator.deallocate(m_data);
        m_length = 0;
        m_capacity = 0;
    }

    void push_back(const T element){
        if(m_length == m_capacity){
            m_capacity += m_capacity / 2;
            T* new_data = allocator.allocate(m_capacity);
            for(size_t i = 0; i < m_length; ++i){
                new_data[i] = m_data[i];
            }
            allocator.deallocate(m_data);
            m_data = new_data;
        }
        m_data[m_length] = element;
        ++m_length;
    }

    void pop_at(const size_t index){
        if(index < 0 || index > m_length){
            throw std::out_of_range("Index is out of bounds!!!");
        }
        if(m_length < m_capacity / 2){
            m_capacity /= 2;
        }
        T* new_data = allocator.allocate(m_capacity);

        for(size_t i = 0, j = 0; i < m_length; ++i){
            if(i != index){
                new_data[j++] = m_data[i];
            }

        }

        allocator.deallocate(m_data);
        m_data = new_data;
        --m_length;
    }

    T operator [](const size_t index) const{
        if(index < 0 || index >= m_length){
            throw std::out_of_range("Index is out of bounds!!!");
        }
        return m_data[index];
    }

    void pop_back(){
        pop_at(m_length - 1);
    }

    size_t getLength() const{
        return m_length;
    }

    void print() const{
        for(size_t i = 0; i < m_length; ++i){
            std::cout << m_data[i] << " ";
        }
        std::cout << std::endl;
    }

    Vector<T>& operator = (const Vector<T> &other){
        if(this == &other){
            return *this;
        }
        m_capacity = other.m_capacity;
        m_length = other.m_length;

        allocator.deallocate(m_data);
        m_data = allocator.allocate(m_capacity);

        for(size_t i = 0; i < m_length; ++i){
            m_data[i] = other.m_data[i];
        }

        return *this;
    }
};

#endif //UNTITLED12_VECTOR_H
