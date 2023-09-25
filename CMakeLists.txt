cmake_minimum_required(VERSION 3.22)
project(untitled12)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled12 main.cpp Allocator.cpp Allocator.h String.cpp String.h Vector.h SoundSample.h Sounds.h Playlist.h)
