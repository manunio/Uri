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
include CMakeFiles/Uri.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Uri.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Uri.dir/flags.make

CMakeFiles/Uri.dir/src/Uri.cpp.o: CMakeFiles/Uri.dir/flags.make
CMakeFiles/Uri.dir/src/Uri.cpp.o: ../src/Uri.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Uri.dir/src/Uri.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Uri.dir/src/Uri.cpp.o -c /home/maxx/dev/cpp/webserver-rfc-7230/Uri/src/Uri.cpp

CMakeFiles/Uri.dir/src/Uri.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Uri.dir/src/Uri.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maxx/dev/cpp/webserver-rfc-7230/Uri/src/Uri.cpp > CMakeFiles/Uri.dir/src/Uri.cpp.i

CMakeFiles/Uri.dir/src/Uri.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Uri.dir/src/Uri.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maxx/dev/cpp/webserver-rfc-7230/Uri/src/Uri.cpp -o CMakeFiles/Uri.dir/src/Uri.cpp.s

# Object files for target Uri
Uri_OBJECTS = \
"CMakeFiles/Uri.dir/src/Uri.cpp.o"

# External object files for target Uri
Uri_EXTERNAL_OBJECTS =

lib/libUri.a: CMakeFiles/Uri.dir/src/Uri.cpp.o
lib/libUri.a: CMakeFiles/Uri.dir/build.make
lib/libUri.a: CMakeFiles/Uri.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library lib/libUri.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Uri.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Uri.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Uri.dir/build: lib/libUri.a

.PHONY : CMakeFiles/Uri.dir/build

CMakeFiles/Uri.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Uri.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Uri.dir/clean

CMakeFiles/Uri.dir/depend:
	cd /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maxx/dev/cpp/webserver-rfc-7230/Uri /home/maxx/dev/cpp/webserver-rfc-7230/Uri /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug /home/maxx/dev/cpp/webserver-rfc-7230/Uri/cmake-build-debug/CMakeFiles/Uri.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Uri.dir/depend

