cmake_minimum_required(VERSION 3.21)

project(libactor VERSION 0.1)
enable_language(CXX)

# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(experiments)
endif()
