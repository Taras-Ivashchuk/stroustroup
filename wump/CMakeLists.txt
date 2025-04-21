cmake_minimum_required(VERSION 3.5)
project(wump LANGUAGES CXX)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} 
	main.cpp
	room.cpp
	cave.cpp
	help.cpp
	game.cpp
	creature.cpp
	room.h
	cave.h
	help.h
	creature.h
	Random.h
	game.h)
