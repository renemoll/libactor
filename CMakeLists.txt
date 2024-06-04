cmake_minimum_required(VERSION 3.21)

#
# [BOB] This is needed right here to download the toolchain files
#       before they are used within a project.
#
list(PREPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
find_package(Bob REQUIRED)

project(libactor
	VERSION
		0.1.0
	LANGUAGES
		CXX C ASM
)

# set(CMAKE_C_STANDARD 17)
# set(CMAKE_C_STANDARD_REQUIRED ON)
# set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(USER_VERSION_HEADER "${CMAKE_CURRENT_LIST_DIR}/cmake/version.in.h")
include(bob)

if (PROJECT_IS_TOP_LEVEL)
	add_subdirectory(experiments)
endif()
