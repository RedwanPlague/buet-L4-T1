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
CMAKE_SOURCE_DIR = /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/attacker.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/attacker.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/attacker.dir/flags.make

CMakeFiles/attacker.dir/attacker.c.o: CMakeFiles/attacker.dir/flags.make
CMakeFiles/attacker.dir/attacker.c.o: ../attacker.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/attacker.dir/attacker.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/attacker.dir/attacker.c.o -c /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/attacker.c

CMakeFiles/attacker.dir/attacker.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/attacker.dir/attacker.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/attacker.c > CMakeFiles/attacker.dir/attacker.c.i

CMakeFiles/attacker.dir/attacker.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/attacker.dir/attacker.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/attacker.c -o CMakeFiles/attacker.dir/attacker.c.s

# Object files for target attacker
attacker_OBJECTS = \
"CMakeFiles/attacker.dir/attacker.c.o"

# External object files for target attacker
attacker_EXTERNAL_OBJECTS =

attacker: CMakeFiles/attacker.dir/attacker.c.o
attacker: CMakeFiles/attacker.dir/build.make
attacker: CMakeFiles/attacker.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable attacker"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/attacker.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/attacker.dir/build: attacker

.PHONY : CMakeFiles/attacker.dir/build

CMakeFiles/attacker.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/attacker.dir/cmake_clean.cmake
.PHONY : CMakeFiles/attacker.dir/clean

CMakeFiles/attacker.dir/depend:
	cd /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug /home/redwan/Documents/L4-T1/all-codes/security-lab/project/spoofing/cmake-build-debug/CMakeFiles/attacker.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/attacker.dir/depend

