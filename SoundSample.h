#ifndef UNTITLED12_SOUNDSAMPLE_H
#define UNTITLED12_SOUNDSAMPLE_H
#include <limits>
#include <iostream>
const int M = 500;

template<class T>
bool Is_Number(){
    if(std::is_arithmetic_v<T>){
        return true;
    }
    return false;
}

template<class T>
class SoundSample {
private:
    T m_amplitude;
    bool is_number;
public:
    SoundSample() : is_number(Is_Number<T>() == 0 ? throw std::invalid_argument("Amplitude and Interval should be numbers!!!") : 1), m_amplitude(0) {
    }
    SoundSample(const T amplitude)
            : is_number(Is_Number<T>() == 0 ? throw std::invalid_argument("Amplitude and Interval should be numbers!!!") : 1),
              m_amplitude(amplitude > M ? M : amplitude < -M ? -M : amplitude){}
    SoundSample(const SoundSample<T> &other) : m_amplitude(other.m_amplitude){}

    const T getAmplitude() const noexcept{
        return m_amplitude;
    }
};

#endif //UNTITLED12_SOUNDSAMPLE_H
