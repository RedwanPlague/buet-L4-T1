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
CMAKE_SOURCE_DIR = /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/mine.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/mine.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/mine.dir/flags.make

CMakeFiles/mine.dir/main.c.o: CMakeFiles/mine.dir/flags.make
CMakeFiles/mine.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/mine.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/mine.dir/main.c.o -c /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/main.c

CMakeFiles/mine.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/mine.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/main.c > CMakeFiles/mine.dir/main.c.i

CMakeFiles/mine.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/mine.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/main.c -o CMakeFiles/mine.dir/main.c.s

# Object files for target mine
mine_OBJECTS = \
"CMakeFiles/mine.dir/main.c.o"

# External object files for target mine
mine_EXTERNAL_OBJECTS =

mine: CMakeFiles/mine.dir/main.c.o
mine: CMakeFiles/mine.dir/build.make
mine: CMakeFiles/mine.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable mine"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/mine.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/mine.dir/build: mine

.PHONY : CMakeFiles/mine.dir/build

CMakeFiles/mine.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mine.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mine.dir/clean

CMakeFiles/mine.dir/depend:
	cd /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/security-lab/project/dhcp/cmake-build-debug/CMakeFiles/mine.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mine.dir/depend

