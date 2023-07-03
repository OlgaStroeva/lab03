cmake_minimum_required(VERSION 3.5)

project(project LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(CURL REQUIRED)

find_package(Qt6 REQUIRED COMPONENTS Core)
INCLUDE_DIRECTORIES("/home/olga/anaconda3/lib")

find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(CURL REQUIRED)

#/home/olga/anaconda3/include/curl

add_executable(project main.cpp
    #histogram.h histogram.cpp
    )

target_link_libraries(project PRIVATE Qt${QT_VERSION_MAJOR}::Network)
target_link_libraries(project PRIVATE curl)
#target_link_libraries(project PRIVATE Qt${QT_VERSION_MAJOR}::QtCUrl)
