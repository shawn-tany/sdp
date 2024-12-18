# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/txy/sdp/components/sdk/pcre/pcre-8.45

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/txy/sdp/components/sdk/pcre/pcre-8.45/build

# Include any dependencies generated for this target.
include CMakeFiles/pcretest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/pcretest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/pcretest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pcretest.dir/flags.make

CMakeFiles/pcretest.dir/pcretest.c.o: CMakeFiles/pcretest.dir/flags.make
CMakeFiles/pcretest.dir/pcretest.c.o: ../pcretest.c
CMakeFiles/pcretest.dir/pcretest.c.o: CMakeFiles/pcretest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/pcretest.dir/pcretest.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/pcretest.dir/pcretest.c.o -MF CMakeFiles/pcretest.dir/pcretest.c.o.d -o CMakeFiles/pcretest.dir/pcretest.c.o -c /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcretest.c

CMakeFiles/pcretest.dir/pcretest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pcretest.dir/pcretest.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcretest.c > CMakeFiles/pcretest.dir/pcretest.c.i

CMakeFiles/pcretest.dir/pcretest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pcretest.dir/pcretest.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcretest.c -o CMakeFiles/pcretest.dir/pcretest.c.s

CMakeFiles/pcretest.dir/pcre_printint.c.o: CMakeFiles/pcretest.dir/flags.make
CMakeFiles/pcretest.dir/pcre_printint.c.o: ../pcre_printint.c
CMakeFiles/pcretest.dir/pcre_printint.c.o: CMakeFiles/pcretest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/pcretest.dir/pcre_printint.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/pcretest.dir/pcre_printint.c.o -MF CMakeFiles/pcretest.dir/pcre_printint.c.o.d -o CMakeFiles/pcretest.dir/pcre_printint.c.o -c /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcre_printint.c

CMakeFiles/pcretest.dir/pcre_printint.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pcretest.dir/pcre_printint.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcre_printint.c > CMakeFiles/pcretest.dir/pcre_printint.c.i

CMakeFiles/pcretest.dir/pcre_printint.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pcretest.dir/pcre_printint.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/txy/sdp/components/sdk/pcre/pcre-8.45/pcre_printint.c -o CMakeFiles/pcretest.dir/pcre_printint.c.s

# Object files for target pcretest
pcretest_OBJECTS = \
"CMakeFiles/pcretest.dir/pcretest.c.o" \
"CMakeFiles/pcretest.dir/pcre_printint.c.o"

# External object files for target pcretest
pcretest_EXTERNAL_OBJECTS =

pcretest: CMakeFiles/pcretest.dir/pcretest.c.o
pcretest: CMakeFiles/pcretest.dir/pcre_printint.c.o
pcretest: CMakeFiles/pcretest.dir/build.make
pcretest: libpcreposix.a
pcretest: libpcre.a
pcretest: CMakeFiles/pcretest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/txy/sdp/components/sdk/pcre/pcre-8.45/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable pcretest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pcretest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pcretest.dir/build: pcretest
.PHONY : CMakeFiles/pcretest.dir/build

CMakeFiles/pcretest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pcretest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pcretest.dir/clean

CMakeFiles/pcretest.dir/depend:
	cd /home/txy/sdp/components/sdk/pcre/pcre-8.45/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/txy/sdp/components/sdk/pcre/pcre-8.45 /home/txy/sdp/components/sdk/pcre/pcre-8.45 /home/txy/sdp/components/sdk/pcre/pcre-8.45/build /home/txy/sdp/components/sdk/pcre/pcre-8.45/build /home/txy/sdp/components/sdk/pcre/pcre-8.45/build/CMakeFiles/pcretest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pcretest.dir/depend

