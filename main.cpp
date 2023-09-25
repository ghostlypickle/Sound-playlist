#include <iostream>
#include "Allocator.h"
#include "String.h"
#include "Vector.h"
#include "SoundSample.h"
#include "Sounds.h"

template<class T>
void save_to_file(const String &filename, const Sound<T> &sound){
    std::ofstream output(filename.getData(), std::ios::app | std::ios::binary);
    if(!output){
        throw std::ios_base::failure("Cannot open file!!!");
    }
    for(size_t i = 0; i < sound.get_lenght(); ++i){
        output.write(reinterpret_cast<const char*>(&sound[i]), sizeof (sound[i]));
    }
    output.close();
}

void delete_file(const String &filename){
    std::ofstream output(filename.getData(), std::ios::trunc);
    output.close();
}

int main() {
    String str1("smthidk");
    String str2("rlydk");
    std::cout << str1 << "    " << str2 << std::endl;
    Vector<String> v;
    v.push_back(str1);
    v.push_back(str2);
    v.print();
    v.pop_back();
    v.print();
    String str3 = str1;
    str3.append(str2);
    v.push_back(str2);
    v.push_back(str3);
    v.pop_at(1);
    v.print();
    std::cout << Is_Number<int>() << std::endl;
    Vector<SoundSample<int>*> sound_vec;
    Allocator<SoundSample<int>> sample_allocator;
    SoundSample<int>* sample1 = sample_allocator.construct(2);
    SoundSample<int>* sample2 = sample_allocator.construct(5);
    SoundSample<int>* sample3 = sample_allocator.construct(7);
    sound_vec.push_back(sample1);
    sound_vec.push_back(sample2);
    sound_vec.push_back(sample3);
    sound_vec.push_back(sample1);
    for(size_t i = 0; i < sound_vec.getLength(); ++i){
        std::cout << sound_vec[i]->getAmplitude() << " " << std::endl;
    }
    std::cout << "Silence: " << std::endl;
    Allocator<Silence<int>> silence_allocator;
    Silence<int>* silence = silence_allocator.construct(3);
    Vector<Silence<int>*> silence_vec;
    silence_vec.push_back(silence);
    silence_vec[0]->print();

    return 0;
}
