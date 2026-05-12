cmake_minimum_required(VERSION 3.16)

project(dnf_minimizer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC OFF)
set(CMAKE_AUTORCC OFF)

find_package(Qt6 COMPONENTS Core REQUIRED)

add_library(minimizer_core
    minimizer.cpp
    minimizer.h
)

target_link_libraries(minimizer_core PRIVATE Qt6::Core)
