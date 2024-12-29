cmake_minimum_required(VERSION 3.5)
project(binary LANGUAGES CXX)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
	# target_compile_options(${PROJECT_NAME} PRIVATE -g)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    # other macOS-specific flags for Clang
endif()

find_package(FLTK REQUIRED NO_MODULE)
include_directories(${FLTK_INCLUDE_DIRS})
message(FLTK_INCLUDE_DIRS = ${FLTK_INCLUDE_DIRS})
add_executable(${PROJECT_NAME} 
		src/${PROJECT_NAME}.cpp
		lib/src/Graph.cpp
		lib/src/GUI.cpp
		lib/src/Simple_window.cpp
		lib/src/Window.cpp )
target_link_libraries(${PROJECT_NAME} fltk fltk_images)  
target_include_directories(${PROJECT_NAME} PUBLIC lib/include/)
# set(CMAKE_VERBOSE_MAKEFILE ON)
