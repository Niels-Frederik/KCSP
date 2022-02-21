cmake_minimum_required(VERSION 3.21)
project(MiniProject1)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(MiniProject1
        Concurrent_Partitioning_Algorithm.cpp
        Concurrent_Partitioning_Algorithm.h
        Independent_Partitioning_Algorithm.cpp
        Independent_Partitioning_Algorithm.h
        Main.cpp
        TupleGenerator.cpp
        TupleGenerator.h)
