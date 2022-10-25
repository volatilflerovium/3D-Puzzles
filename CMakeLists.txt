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
	animation.cpp
	base.cpp
#	reference_frame.cpp
	camera.cpp
	shape.cpp
	cube.cpp
	tetra.cpp
	rubik.cpp
	dogic.cpp
	dogic12.cpp
	modulo.cpp
	state_machine.cpp
	game_state.cpp
	game_state_machine.cpp
	button.cpp
	utilities.cpp
	user_input.cpp
)

#MESSAGE("SFML +lib : ${SFML_LIBRARIES}") 

add_executable(puzzle-app ${SOURCES})

target_link_libraries(puzzle-app sfml-system sfml-window sfml-graphics )

# install resources
#add_subdirectory(resources)
