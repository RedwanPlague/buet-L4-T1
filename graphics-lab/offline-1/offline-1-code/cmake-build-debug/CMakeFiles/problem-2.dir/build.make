# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/redwan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.7442.42/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/redwan/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.7442.42/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/problem-2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/problem-2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/problem-2.dir/flags.make

CMakeFiles/problem-2.dir/problem-2.cpp.o: CMakeFiles/problem-2.dir/flags.make
CMakeFiles/problem-2.dir/problem-2.cpp.o: ../problem-2.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/problem-2.dir/problem-2.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/problem-2.dir/problem-2.cpp.o -c /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/problem-2.cpp

CMakeFiles/problem-2.dir/problem-2.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/problem-2.dir/problem-2.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/problem-2.cpp > CMakeFiles/problem-2.dir/problem-2.cpp.i

CMakeFiles/problem-2.dir/problem-2.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/problem-2.dir/problem-2.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/problem-2.cpp -o CMakeFiles/problem-2.dir/problem-2.cpp.s

# Object files for target problem-2
problem__2_OBJECTS = \
"CMakeFiles/problem-2.dir/problem-2.cpp.o"

# External object files for target problem-2
problem__2_EXTERNAL_OBJECTS =

problem-2: CMakeFiles/problem-2.dir/problem-2.cpp.o
problem-2: CMakeFiles/problem-2.dir/build.make
problem-2: CMakeFiles/problem-2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable problem-2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/problem-2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/problem-2.dir/build: problem-2

.PHONY : CMakeFiles/problem-2.dir/build

CMakeFiles/problem-2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/problem-2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/problem-2.dir/clean

CMakeFiles/problem-2.dir/depend:
	cd /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/graphics-lab/offline-1/offline-1-code/cmake-build-debug/CMakeFiles/problem-2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/problem-2.dir/depend

