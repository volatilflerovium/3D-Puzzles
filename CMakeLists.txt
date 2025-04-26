cmake_minimum_required(VERSION 3.13.4)

project(Puzzle)

# --------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------------------

if(NDEBUG)
	add_definitions(-DNDEBUG=1)
endif() 

if(NOT(CMAKE_BUILD_TYPE))
	set(CMAKE_BUILD_TYPE "Debug")
	add_definitions(-DDEBUG)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
	add_compile_options("-Wno-comment")
	add_definitions(-DDEBUG_DIR="${CMAKE_SOURCE_DIR}")
endif()

add_compile_options("-O3")

##--------------------------------------------------------------------

find_package(Threads REQUIRED)

##--------------------------------------------------------------------

find_package(SFML 2.5.1 COMPONENTS system window graphics REQUIRED)

# Show a message with the opencv version detected 
message("SFML version : ${SFML_VERSION}") 

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

#add_compile_options(-Wall -Wextra -pedantic -Werror)

add_executable(puzzle-app
	src/animation.cpp
	src/base.cpp
	src/camera.cpp
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
)

target_include_directories(
	puzzle-app
	PRIVATE
	"include"
)

target_link_libraries(puzzle-app sfml-system sfml-window sfml-graphics )

# install resources
add_subdirectory(resources)
