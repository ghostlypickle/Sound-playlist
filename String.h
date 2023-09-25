#ifndef UNTITLED12_STRING_H
#define UNTITLED12_STRING_H
#include "Allocator.h"
#include <iostream>
#include <cstring>

class String {
private:
    char* m_data;
    size_t m_length;
    Allocator<char> allocator;

public:

    String();

    String(const char* str);

    String(const String &other);

    String(String &&other) noexcept;

    ~String();

    void clear();

    char operator [](const size_t index) const;

    const size_t getLength() const noexcept;

    friend std::ostream& operator <<(std::ostream& out, const String& str){
        if(str.m_data != nullptr){
            out << str.m_data;
        }
        return out;
    }

    friend std::istream& operator >>(std::istream& in, String& str) {
        delete[] str.m_data;
        char buffer[1024];
        in.getline(buffer, 1024);
        str.m_length = strlen(buffer);
        str.m_data = new char[str.m_length + 1];
        strcpy(str.m_data, buffer);
        str.m_data[str.m_length] = '\0';

        return in;
    }

    String& operator =(const String& str);

    void append(const String& left);

    bool operator ==(const String& str);

    bool operator !=(const String& str);

    char* getData() const;
};


#endif //UNTITLED12_STRING_H
