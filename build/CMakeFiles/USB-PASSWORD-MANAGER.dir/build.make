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
CMAKE_SOURCE_DIR = /home/meganuts/projects/USB-Password-Manager

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/meganuts/projects/USB-Password-Manager/build

# Include any dependencies generated for this target.
include CMakeFiles/USB-PASSWORD-MANAGER.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/USB-PASSWORD-MANAGER.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/USB-PASSWORD-MANAGER.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/USB-PASSWORD-MANAGER.dir/flags.make

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o: CMakeFiles/USB-PASSWORD-MANAGER.dir/flags.make
CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o: CMakeFiles/USB-PASSWORD-MANAGER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/meganuts/projects/USB-Password-Manager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o -MF CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o.d -o CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o -c /home/meganuts/projects/USB-Password-Manager/src/main.cpp

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/meganuts/projects/USB-Password-Manager/src/main.cpp > CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.i

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/meganuts/projects/USB-Password-Manager/src/main.cpp -o CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.s

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o: CMakeFiles/USB-PASSWORD-MANAGER.dir/flags.make
CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o: ../src/core/EncryptionUtil.cpp
CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o: CMakeFiles/USB-PASSWORD-MANAGER.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/meganuts/projects/USB-Password-Manager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o -MF CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o.d -o CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o -c /home/meganuts/projects/USB-Password-Manager/src/core/EncryptionUtil.cpp

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/meganuts/projects/USB-Password-Manager/src/core/EncryptionUtil.cpp > CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.i

CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/meganuts/projects/USB-Password-Manager/src/core/EncryptionUtil.cpp -o CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.s

# Object files for target USB-PASSWORD-MANAGER
USB__PASSWORD__MANAGER_OBJECTS = \
"CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o" \
"CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o"

# External object files for target USB-PASSWORD-MANAGER
USB__PASSWORD__MANAGER_EXTERNAL_OBJECTS =

USB-PASSWORD-MANAGER: CMakeFiles/USB-PASSWORD-MANAGER.dir/src/main.cpp.o
USB-PASSWORD-MANAGER: CMakeFiles/USB-PASSWORD-MANAGER.dir/src/core/EncryptionUtil.cpp.o
USB-PASSWORD-MANAGER: CMakeFiles/USB-PASSWORD-MANAGER.dir/build.make
USB-PASSWORD-MANAGER: CMakeFiles/USB-PASSWORD-MANAGER.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/meganuts/projects/USB-Password-Manager/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable USB-PASSWORD-MANAGER"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/USB-PASSWORD-MANAGER.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/USB-PASSWORD-MANAGER.dir/build: USB-PASSWORD-MANAGER
.PHONY : CMakeFiles/USB-PASSWORD-MANAGER.dir/build

CMakeFiles/USB-PASSWORD-MANAGER.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/USB-PASSWORD-MANAGER.dir/cmake_clean.cmake
.PHONY : CMakeFiles/USB-PASSWORD-MANAGER.dir/clean

CMakeFiles/USB-PASSWORD-MANAGER.dir/depend:
	cd /home/meganuts/projects/USB-Password-Manager/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/meganuts/projects/USB-Password-Manager /home/meganuts/projects/USB-Password-Manager /home/meganuts/projects/USB-Password-Manager/build /home/meganuts/projects/USB-Password-Manager/build /home/meganuts/projects/USB-Password-Manager/build/CMakeFiles/USB-PASSWORD-MANAGER.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/USB-PASSWORD-MANAGER.dir/depend

