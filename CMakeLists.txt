  
cmake_minimum_required(VERSION 3.16)

project(RISC_V_Simulator_1.5)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        main.cpp
        transfer.hpp
        classes.hpp
        analysis.hpp
        carried.hpp
        loads.hpp
        prophecy.hpp
        )