#ifndef UNTITLED12_PLAYLIST_H
#define UNTITLED12_PLAYLIST_H
#include <iostream>
#include <fstream>
#include "Allocator.h"
#include "String.h"
#include "Vector.h"
#include "SoundSample.h"
#include "Sounds.h"

template<class T>
class Playlist{
private:
    Vector<Mix<T>> songs;
public:
    Playlist(){}
    Playlist(const String &filename){}
    Playlist(const Playlist<T> &other) : songs(other.songs){}
    ~Playlist(){
        Allocator<Sound<T>> allocator;
        for(size_t i = 0; i < songs.getLength(); ++i){
            allocator.destroy(songs[i]);
        }
    }

    Sound<T> * operator[](const size_t index){
        if(index < 0 || index >= songs.getLength()){
            throw std::out_of_range("Index of song is out of range!!!");
        }
        for(size_t i = 0; i < songs.getLength(); ++i){
            if(i == index){
                return songs[i];
            }
        }
    }

};

#endif //UNTITLED12_PLAYLIST_H
