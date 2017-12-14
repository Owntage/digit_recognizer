cmake_minimum_required(VERSION 2.8)
project(network_shooter_project)

add_executable(perceptron)

add_definitions(-std=c++11 -g)

set(SFML_ROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(SFML REQUIRED system window graphics network audio)

if(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})	
	target_link_libraries(perceptron ${SFML_LIBRARIES})
	if(WIN32)
  		target_link_libraries(network wsock32 ws2_32)
	endif(WIN32)
endif()




