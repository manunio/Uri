# CMakeLists.txt for Uri
#
# © 2018 by Richard Walters

cmake_minimum_required(VERSION 3.8)

project(Uri CXX)

set(This Uri)

set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

include(cmake/Conan.cmake)
run_conan()

set(Headers
    include/Uri/Uri.hpp
)

set(Sources
    src/Uri.cpp
)

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
    FOLDER Libraries
)

target_include_directories(${This} PUBLIC include)

add_subdirectory(test)
