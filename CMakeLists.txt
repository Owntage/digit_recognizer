cmake_minimum_required(VERSION 2.8)
project(digit_recognizer)

set(MY_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(MY_SOURCES ${MY_SOURCE_DIR}/main.cpp)

add_executable(perceptron ${MY_SOURCES} source/declarations.h source/net.cpp source/neuron.cpp source/pixel_field.h source/minist_loader.h)

add_definitions(-std=c++11 -g)

set(SFML_ROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(SFML REQUIRED system window graphics network audio)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})	
	target_link_libraries(perceptron ${SFML_LIBRARIES})
endif()




