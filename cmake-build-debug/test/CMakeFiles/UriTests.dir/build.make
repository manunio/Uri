# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2020.1/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.1/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maxx/dev/cpp/webserver-rfc-7230/Uri

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug

# Include any dependencies generated for this target.
include test/CMakeFiles/UriTests.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/UriTests.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/UriTests.dir/flags.make

test/CMakeFiles/UriTests.dir/src/UriTests.cpp.o: test/CMakeFiles/UriTests.dir/flags.make
test/CMakeFiles/UriTests.dir/src/UriTests.cpp.o: ../test/src/UriTests.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/UriTests.dir/src/UriTests.cpp.o"
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/UriTests.dir/src/UriTests.cpp.o -c /home/maxx/dev/cpp/webserver-rfc-7230/Uri/test/src/UriTests.cpp

test/CMakeFiles/UriTests.dir/src/UriTests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UriTests.dir/src/UriTests.cpp.i"
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxx/dev/cpp/webserver-rfc-7230/Uri/test/src/UriTests.cpp > CMakeFiles/UriTests.dir/src/UriTests.cpp.i

test/CMakeFiles/UriTests.dir/src/UriTests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UriTests.dir/src/UriTests.cpp.s"
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxx/dev/cpp/webserver-rfc-7230/Uri/test/src/UriTests.cpp -o CMakeFiles/UriTests.dir/src/UriTests.cpp.s

# Object files for target UriTests
UriTests_OBJECTS = \
"CMakeFiles/UriTests.dir/src/UriTests.cpp.o"

# External object files for target UriTests
UriTests_EXTERNAL_OBJECTS =

bin/UriTests: test/CMakeFiles/UriTests.dir/src/UriTests.cpp.o
bin/UriTests: test/CMakeFiles/UriTests.dir/build.make
bin/UriTests: lib/libUri.a
bin/UriTests: /home/maxx/.conan/data/gtest/1.10.0/_/_/package/ec1caa886f1802381c5ecbb31ff59dda4dcdd8ce/lib/libgtest_maind.a
bin/UriTests: /home/maxx/.conan/data/gtest/1.10.0/_/_/package/ec1caa886f1802381c5ecbb31ff59dda4dcdd8ce/lib/libgmock_maind.a
bin/UriTests: /home/maxx/.conan/data/gtest/1.10.0/_/_/package/ec1caa886f1802381c5ecbb31ff59dda4dcdd8ce/lib/libgmockd.a
bin/UriTests: /home/maxx/.conan/data/gtest/1.10.0/_/_/package/ec1caa886f1802381c5ecbb31ff59dda4dcdd8ce/lib/libgtestd.a
bin/UriTests: test/CMakeFiles/UriTests.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/UriTests"
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UriTests.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/UriTests.dir/build: bin/UriTests

.PHONY : test/CMakeFiles/UriTests.dir/build

test/CMakeFiles/UriTests.dir/clean:
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/UriTests.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/UriTests.dir/clean

test/CMakeFiles/UriTests.dir/depend:
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxx/dev/cpp/webserver-rfc-7230/Uri /home/maxx/dev/cpp/webserver-rfc-7230/Uri/test /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/test/CMakeFiles/UriTests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/UriTests.dir/depend

