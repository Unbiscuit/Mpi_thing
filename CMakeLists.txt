cmake_minimum_required(VERSION 3.26)
project(Mpi_thing)

include_directories(/usr/include/x86_64-linux-gnu/mpi)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_C_COMPILER /usr/bin/mpicc)
set(CMAKE_CXX_COMPILER /usr/bin/mpic++)

add_executable(Mpi_thing mpi-examples.cpp)
