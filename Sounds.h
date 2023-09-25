#ifndef UNTITLED12_SOUNDS_H
#define UNTITLED12_SOUNDS_H
#include <iostream>
#include <fstream>
#include "Allocator.h"
#include "String.h"
#include "Vector.h"
#include "SoundSample.h"

template<class T>
class Sound{
protected:
    size_t sound_length;
    Vector<SoundSample<T>*> sound_samples;

    virtual void set_Sound_Samples(Vector<SoundSample<T>*> &samples, const size_t length){
        if(samples.getLength() != 0){
            samples.clear();
        }
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < length; ++i){
            T amplitude;
            std::cout << "Enter value of an amplitude: ";
            std::cin >> amplitude;
            std::cout << std::endl;
            SoundSample<T>* temp = allocator.template construct(amplitude);
            samples.push_back(temp);
        }
    }
public:
    Sound() : sound_length(0){}
    Sound(const size_t interval , const size_t length) : sound_length(length){
        set_Sound_Samples(sound_samples,sound_length);
    }
    Sound(const Sound<T> &other) :sound_length(other.sound_length), sound_samples(other.sound_samples){}

    virtual ~Sound(){
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < sound_length; ++i){
            allocator.deallocate(sound_samples[i]);
        }
    }
    const size_t get_lenght() const noexcept{
        return sound_length;
    }

    const T operator [](size_t index) const{
        return sound_samples[index]->getAmplitude();
    }

    void print() const{
        for(size_t i = 0; i < sound_length; ++i){
            std::cout << sound_samples[i]->getAmplitude() << " "<< std::endl;
        }
    }
};

template<class T>
class Silence : public Sound<T>{
private:
    void set_Sound_Samples(Vector<SoundSample<T>*> &samples, const size_t length) override{
        if(length < 0){
            return;
        }
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < length; ++i){
            SoundSample<T>* temp = allocator.template construct(0);
            samples.push_back(temp);
        }
    }
public:
    Silence() : Sound<T>(0){}
    Silence(const size_t length){
        Sound<T>::sound_length = length;
        set_Sound_Samples(Sound<T>::sound_samples, Sound<T>::sound_length);
    };
};

template<class T>
class Sound_from_file : public Sound<T>{
public:
    Sound_from_file() : Sound<T>(0){}
    Sound_from_file(const String &filename) {
        std::ifstream input(filename.getData(), std::ios::binary);
        if(!input){
            throw std::ios_base::failure("Cannot open file!!!");
        }
        Allocator<SoundSample<T>> allocator;
        T sample_data;

        while (input.read(reinterpret_cast<char*>(&sample_data), sizeof(T))) {
            SoundSample<T>* temp = allocator.template construct(sample_data);
            Sound<T>::sound_samples.push_back(temp);
        }
        Sound<T>::sound_length = Sound<T>::sound_samples.getLength();

        input.close();
    }
};

template<class T>
class Periodical_Sound : Sound<T>{
private:
    size_t m_patern_rep;
    Vector<SoundSample<T>*> m_patern;

    void set_Sound_Samples(Vector<SoundSample<T>*> &samples, size_t length) override{
        length = m_patern.getLength() * m_patern_rep;
        for(size_t i = 0; i < m_patern_rep; ++i){
            for(size_t j = 0; j < m_patern.getLength(); ++j){
                samples.push_back(m_patern[j]);
            }
        }
    }
public:
    Periodical_Sound() : m_patern_rep (0), Sound<T>(0) {};
    Periodical_Sound(const Vector<SoundSample<T>> &patern, const size_t reps) : m_patern(patern), m_patern_rep(reps){
        set_Sound_Samples(Sound<T>::sound_samples, Sound<T>::sound_length);
    }
    ~Periodical_Sound() override{
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < m_patern.getLength(); ++i){
            allocator.deallocate(m_patern[i]);
        }
    }

};

template<class T>
class Mix : Sound<T>{
private:
    Vector<Sound<T>*> m_sounds;

    void set_Sound_Samples(Vector<SoundSample<T>> &samples, size_t length) override{
        length = getMax_Length();
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < length; ++i){
            if(i >= m_sounds.getLength()){
                SoundSample<T>* temp = allocator.template construct();
                samples.push_back(temp);
            }
            else{
                samples.push_back(form_Sample(m_sounds[i]));
            }
        }
    }

public:
    Mix() : Sound<T>(0){}
    Mix(const Vector<Sound<T>> &sounds){
        set_Sound_Samples(Sound<T>::sound_samples, Sound<T>::sound_length);
    }
    ~Mix() override{
        Allocator<Sound<T>> allocator;
        for(size_t i = 0; i < m_sounds.getLength(); ++i){
            allocator.destroy(m_sounds[i]);
        }
    }

private:
    const size_t getMax_Length(){
        size_t max = 0;
        for(size_t i = 0; i < m_sounds.getLength(); ++i){
            if(m_sounds[i]->sound_length > max){
                max = m_sounds[i]->sound_length;
            }
        }

        return max;
    }

    const SoundSample<T> *&form_Sample(const Sound<T> *&sound){
        T amp = 0;
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < sound->sound_length; ++i){
            amp += sound->sound_samples[i]->getAmplitude();
        }
        SoundSample<T> *temp = allocator.template construct(amp / sound->sound_length);

        return  temp;
    };
};

template<class T>
class Continuous_Sound : Sound<T>{
private:
    Vector<Sound<T>*> m_sounds;

    void set_Sound_Samples(Vector<SoundSample<T>*> &samples, size_t length) override{
        length = get_Sounds_length();
        Allocator<SoundSample<T>> allocator;
        for(size_t i = 0; i < m_sounds.getLength(); ++i){
            for(size_t j = 0; j < m_sounds[i]->sound_length; ++j){
                SoundSample<T> *temp = allocator.template construct(m_sounds[i]->sound_samples[j]->getAmplitude());
                samples.push_back(temp);
            }
        }
    }

public:
    Continuous_Sound() : Sound<T>(0){}
    Continuous_Sound(const Vector<Sound<T>> &sounds){
        set_Sound_Samples(Sound<T>::sound_samples, Sound<T>::sound_length);
    }
    ~Continuous_Sound() override{
        Allocator<Sound<T>> allocator;
        for(size_t i = 0; i < m_sounds.getLength(); ++i){
            allocator.destroy(m_sounds[i]);
        }
    }

private:
    const size_t get_Sounds_length() const{
        size_t length = 0;
        for(size_t i = 0; i < m_sounds.getLength(); ++i){
            length += m_sounds[i]->sound_length;
        }

        return length;
    }
};

template<class T>
class Effect : public Sound<T>{
public:
    using DesiredEfect = void (*)(const SoundSample<T>*, const size_t);
    DesiredEfect effect;

    Effect() : Sound<T>(0){}
    Effect(const Sound<T> &sound, DesiredEfect effect_to_apply) : effect(effect_to_apply){
        Sound<T>::sound_length = sound.sound_length;
        Sound<T>::sound_samples = sound.sound_samples;
        std::cout<< "Enter 1 to apply change for 1 sample only and 2 for the whole sound:" << " ";
        size_t application;
        std::cin >> application;
        std::cout << std::endl;
        if(application == 1 || application == 2){
            switch (application) {
                case 1:
                    size_t index;
                    std::cout << "Enter index of the sample that will be changed:" << " ";
                    std::cin >> index;
                    std::cout << std::endl;
                    if(index < 0 || index >= Sound<T>::sound_length){
                        throw std::out_of_range("Index is out of range!!!");
                    }
                    effect(Sound<T>::sound_samples[index], index);
                case 2:
                    for(size_t i = 0; i < Sound<T>::sound_length; ++i){
                        effect(Sound<T>::sound_samples[i],i);
                    }
                    break;
            }
        }
        else{
            std::cout << "Invalid choice, no changes were applied." << std::endl;
        }


    }

    void mute(const SoundSample<T> *sample, const size_t index){
        Allocator<SoundSample<T>> allocator;
        SoundSample<T>* changed_sample = allocator.template construct(0);
        Sound<T>::sound_samples[index] = changed_sample;
    }

    void volume_change(const SoundSample<T> *sample, const size_t index){
        Allocator<SoundSample<T>> allocator;
        double volume_coeficent;
        std::cout << "Enter the coeficent by which the sample will be changed :" << " ";
        std::cin >> volume_coeficent;
        std::cout << std::endl;

        SoundSample<double> *changed_sample = allocator.template construct(static_cast<double>(sample->getAmplitude()*volume_coeficent));
        Sound<T>::sound_samples[index] = changed_sample;
    }

    void oposite_of_sample(const SoundSample<T> *sample, const size_t index){
        Allocator<SoundSample<T>> allocator;
        SoundSample<T>* changed_sample = allocator.template construct(sample->getAmplitude()*(-1));
        Sound<T>::sound_samples[index] = changed_sample;
    }
};

#endif //UNTITLED12_SOUNDS_H
