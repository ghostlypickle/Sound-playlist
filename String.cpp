#include "String.h"

String::String()  : m_data(nullptr), m_length(0) {
    m_data = allocator.allocate(1);
    m_data[0] = '\0';
}

String::String(const char *str) {
    if(str == nullptr){
        m_data = allocator.allocate(1);
        m_length = 0;
        m_data[0] = '\0';
    }
    m_length = strlen(str);
    m_data = allocator.allocate(m_length + 1);
    strcpy(m_data,str);
    m_data[m_length] = '\0';
}

String::String(const String &other){
    m_length = other.m_length;
    m_data = new char[m_length + 1];
    strcpy(m_data,other.m_data);
    m_data[m_length] = '\0';
}

String::String(String &&other) noexcept : m_data(other.m_data), m_length(other.m_length){
other.m_data = nullptr;
other.m_length = 0;
}

String::~String(){
    clear();
}

const size_t String::getLength() const noexcept {
    return m_length;
}

char *String::getData() const {
    return m_data;
}

void String::append(const String &left) {
    char* tempData = new char[m_length + 1];
    strcpy(tempData,m_data);
    tempData[m_length] = '\0';
    m_length += left.m_length;
    delete[] m_data;
    m_data = new char[m_length + 1];
    strcpy(m_data,tempData);
    strcpy(m_data + strlen(tempData),left.m_data);
    delete[] tempData;
    m_data[m_length] = '\0';
}

void String::clear() {
    allocator.deallocate(m_data);
    m_length = 0;
}

String &String::operator=(const String &str) {
    if(this == &str){
        return *this;
    }
    m_length = str.m_length;
    delete[] m_data;
    m_data = new char[m_length + 1];
    strcpy(m_data,str.m_data);

    return *this;
}

bool String::operator==(const String &str) {
    if(m_length != str.m_length){
        return false;
    }
    for(size_t i = 0 ;i < m_length; ++i){
        if(m_data[i] != str.m_data[i]){
            return false;
        }
    }

    return true;
}

bool String::operator!=(const String &str) {
    return !(*this == str);
}

char String::operator[](const size_t index) const {
    if(index < 0 || index > m_length){
        throw std::out_of_range("The provided index is out of range!!!");
    }
    return m_data[index];
}