# CMakeLists.txt for UriTests
#
# © 2021 Manu Nair

cmake_minimum_required(VERSION 3.8)
set(This UriTests)

set(Sources
    src/UriTests.cpp
)

add_executable(${This} ${Sources})
set_target_properties(${This} PROPERTIES
    FOLDER Tests
)

target_link_libraries(${This} PUBLIC
    CONAN_PKG::gtest
    Uri
)

add_test(
    NAME ${This}
    COMMAND ${This}
)
