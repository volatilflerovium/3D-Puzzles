cmake_minimum_required(VERSION 3.13.4)

# Project's name
project(Puzzle)

FIND_PACKAGE(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)# network audio)

# Show a message with the opencv version detected 
MESSAGE("SFML version : ${SFML_VERSION}") 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#add_compile_options(-Wall -Wextra -pedantic -Werror)

set(release 1)

if(release)
	add_definitions(-DNDEBUG)
endif()

set(SOURCES
	src/animation.cpp
	src/base.cpp
#	reference_frame.cpp
	src/camera.cpp
	src/shape.cpp
	src/cube.cpp
	src/tetra.cpp
	src/rubik.cpp
	src/dogic.cpp
	src/dogic12.cpp
	src/modulo.cpp
	src/state_machine.cpp
	src/game_state.cpp
	src/game_state_machine.cpp
	src/button.cpp
	src/utilities.cpp
	src/user_input.cpp
)

#MESSAGE("SFML +lib : ${SFML_LIBRARIES}") 

add_executable(puzzle-app ${SOURCES})

target_link_libraries(puzzle-app sfml-system sfml-window sfml-graphics )

# install resources
add_subdirectory(resources)
