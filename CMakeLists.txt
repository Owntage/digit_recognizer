cmake_minimum_required(VERSION 2.8)
project(network_shooter_project)

set(MY_SOURCE_DIR ${PROJECT_SOURCE_DIR}/source)
set(MY_SOURCES ${MY_SOURCE_DIR}/neural-net.cpp)

add_executable(perceptron ${MY_SOURCES})

add_definitions(-std=c++11 -g)

set(SFML_ROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(SFML REQUIRED system window graphics network audio)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})	
	target_link_libraries(perceptron ${SFML_LIBRARIES})
endif()




