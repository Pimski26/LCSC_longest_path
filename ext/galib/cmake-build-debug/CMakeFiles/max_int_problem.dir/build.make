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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/max_int_problem.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/max_int_problem.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/max_int_problem.dir/flags.make

CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o: CMakeFiles/max_int_problem.dir/flags.make
CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o: ../app/max_int_problem/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o -c /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/app/max_int_problem/main.cpp

CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/app/max_int_problem/main.cpp > CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.i

CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/app/max_int_problem/main.cpp -o CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.s

# Object files for target max_int_problem
max_int_problem_OBJECTS = \
"CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o"

# External object files for target max_int_problem
max_int_problem_EXTERNAL_OBJECTS =

max_int_problem: CMakeFiles/max_int_problem.dir/app/max_int_problem/main.cpp.o
max_int_problem: CMakeFiles/max_int_problem.dir/build.make
max_int_problem: CMakeFiles/max_int_problem.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable max_int_problem"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/max_int_problem.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/max_int_problem.dir/build: max_int_problem

.PHONY : CMakeFiles/max_int_problem.dir/build

CMakeFiles/max_int_problem.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/max_int_problem.dir/cmake_clean.cmake
.PHONY : CMakeFiles/max_int_problem.dir/clean

CMakeFiles/max_int_problem.dir/depend:
	cd /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug /mnt/c/Users/User.PIM-ZBOOK/CLionProjects/galib/cmake-build-debug/CMakeFiles/max_int_problem.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/max_int_problem.dir/depend

