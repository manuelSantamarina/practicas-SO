# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /snap/clion/169/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/169/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/groug/udc/so/practicas-SO

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/groug/udc/so/practicas-SO/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SO.dir/depend.make
# Include the progress variables for this target.
include CMakeFiles/SO.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SO.dir/flags.make

CMakeFiles/SO.dir/p0.c.o: CMakeFiles/SO.dir/flags.make
CMakeFiles/SO.dir/p0.c.o: ../p0.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/groug/udc/so/practicas-SO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SO.dir/p0.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SO.dir/p0.c.o -c /home/groug/udc/so/practicas-SO/p0.c

CMakeFiles/SO.dir/p0.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SO.dir/p0.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/groug/udc/so/practicas-SO/p0.c > CMakeFiles/SO.dir/p0.c.i

CMakeFiles/SO.dir/p0.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SO.dir/p0.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/groug/udc/so/practicas-SO/p0.c -o CMakeFiles/SO.dir/p0.c.s

# Object files for target SO
SO_OBJECTS = \
"CMakeFiles/SO.dir/p0.c.o"

# External object files for target SO
SO_EXTERNAL_OBJECTS =

SO: CMakeFiles/SO.dir/p0.c.o
SO: CMakeFiles/SO.dir/build.make
SO: CMakeFiles/SO.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/groug/udc/so/practicas-SO/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SO"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SO.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SO.dir/build: SO
.PHONY : CMakeFiles/SO.dir/build

CMakeFiles/SO.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SO.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SO.dir/clean

CMakeFiles/SO.dir/depend:
	cd /home/groug/udc/so/practicas-SO/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/groug/udc/so/practicas-SO /home/groug/udc/so/practicas-SO /home/groug/udc/so/practicas-SO/cmake-build-debug /home/groug/udc/so/practicas-SO/cmake-build-debug /home/groug/udc/so/practicas-SO/cmake-build-debug/CMakeFiles/SO.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SO.dir/depend
