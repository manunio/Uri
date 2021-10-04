# CMakeLists.txt for Uri
#
# © 2021 Manu Nair

cmake_minimum_required(VERSION 3.8)

project(Uri CXX)
include(cmake/StandardProjectSettings.cmake)
include(cmake/PreventInSourceBuilds.cmake)

add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_17)

if (CMAKE_COMPILER_ID MATCHES ".*Clang")
    option(ENABLE_BUILD_WITH_TIME_TRACE "Enable -ftime-trace to generate time tracing .json files on clang" OFF)
    if (ENABLE_BUILD_WITH_TIME_TRACE)
        add_compile_definitions(project_options INTERFACE -ftime-trace)
    endif ()
endif ()

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)

# enable cache system
#TODO: build/install cache
#include(cmake/Cache.cmake)

# standard compiler warnings
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# sanitizer options if supported by compiler
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# enable doxygen
include(cmake/Doxygen.cmake)
enable_doxygen()

# allow for static analysis options
include(cmake/StaticAnalyzers.cmake)

option(BUILD_SHARED_LIBS "Enable compilation of shared libraries" OFF)
option(ENABLE_TESTING "Enable Test Builds" ON)
option(ENABLE_FUZZING "Enable Fuzzing Builds" OFF)

# Very basic PCH example
option(ENABLE_PCH "Enable Precompiled Headers" OFF)
if (ENABLE_PCH)
    # This sets a global PCH parameter, each project will build its own PCH, which
    # is a good idea if any #define's change
    #
    # consider breaking this out per project as necessary
    target_precompile_headers(project_options INTERFACE <vector> <string> <map> <utility>)
endif ()

set(This Uri)

# Set up some extra Conan dependencies based on our needs
# before loading Conan
set(CONAN_EXTRA_REQUIRES "")
set(CONAN_EXTRA_OPTIONS "")

include(cmake/Conan.cmake)
run_conan()

set(Headers
        include/Uri/Uri.hpp
        src/PercentEncodedCharacterDecoder.hpp
        src/IsCharacterInSet.hpp
        )

set(Sources
        src/Uri.cpp
        src/PercentEncodedCharacterDecoder.cpp
        src/IsCharacterInSet.cpp
        )

add_library(${This} STATIC ${Sources} ${Headers})
set_target_properties(${This} PROPERTIES
        FOLDER Libraries
        )

target_include_directories(${This} PUBLIC include)

add_subdirectory(test)
