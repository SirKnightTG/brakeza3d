# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CMake.app/Contents/bin/cmake

# The command to remove a file.
RM = /Applications/CMake.app/Contents/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/darkhead/CLionProjects/recastnavigation/Recast

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/darkhead/CLionProjects/recastnavigation/Recast

# Include any dependencies generated for this target.
include CMakeFiles/Recast.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Recast.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Recast.dir/flags.make

CMakeFiles/Recast.dir/Source/Recast.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/Recast.o: Source/Recast.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Recast.dir/Source/Recast.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/Recast.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/Recast.cpp

CMakeFiles/Recast.dir/Source/Recast.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/Recast.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/Recast.cpp > CMakeFiles/Recast.dir/Source/Recast.i

CMakeFiles/Recast.dir/Source/Recast.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/Recast.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/Recast.cpp -o CMakeFiles/Recast.dir/Source/Recast.s

CMakeFiles/Recast.dir/Source/RecastAlloc.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastAlloc.o: Source/RecastAlloc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Recast.dir/Source/RecastAlloc.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastAlloc.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAlloc.cpp

CMakeFiles/Recast.dir/Source/RecastAlloc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastAlloc.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAlloc.cpp > CMakeFiles/Recast.dir/Source/RecastAlloc.i

CMakeFiles/Recast.dir/Source/RecastAlloc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastAlloc.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAlloc.cpp -o CMakeFiles/Recast.dir/Source/RecastAlloc.s

CMakeFiles/Recast.dir/Source/RecastArea.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastArea.o: Source/RecastArea.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Recast.dir/Source/RecastArea.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastArea.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastArea.cpp

CMakeFiles/Recast.dir/Source/RecastArea.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastArea.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastArea.cpp > CMakeFiles/Recast.dir/Source/RecastArea.i

CMakeFiles/Recast.dir/Source/RecastArea.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastArea.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastArea.cpp -o CMakeFiles/Recast.dir/Source/RecastArea.s

CMakeFiles/Recast.dir/Source/RecastAssert.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastAssert.o: Source/RecastAssert.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Recast.dir/Source/RecastAssert.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastAssert.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAssert.cpp

CMakeFiles/Recast.dir/Source/RecastAssert.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastAssert.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAssert.cpp > CMakeFiles/Recast.dir/Source/RecastAssert.i

CMakeFiles/Recast.dir/Source/RecastAssert.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastAssert.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastAssert.cpp -o CMakeFiles/Recast.dir/Source/RecastAssert.s

CMakeFiles/Recast.dir/Source/RecastContour.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastContour.o: Source/RecastContour.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Recast.dir/Source/RecastContour.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastContour.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastContour.cpp

CMakeFiles/Recast.dir/Source/RecastContour.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastContour.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastContour.cpp > CMakeFiles/Recast.dir/Source/RecastContour.i

CMakeFiles/Recast.dir/Source/RecastContour.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastContour.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastContour.cpp -o CMakeFiles/Recast.dir/Source/RecastContour.s

CMakeFiles/Recast.dir/Source/RecastFilter.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastFilter.o: Source/RecastFilter.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Recast.dir/Source/RecastFilter.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastFilter.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastFilter.cpp

CMakeFiles/Recast.dir/Source/RecastFilter.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastFilter.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastFilter.cpp > CMakeFiles/Recast.dir/Source/RecastFilter.i

CMakeFiles/Recast.dir/Source/RecastFilter.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastFilter.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastFilter.cpp -o CMakeFiles/Recast.dir/Source/RecastFilter.s

CMakeFiles/Recast.dir/Source/RecastLayers.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastLayers.o: Source/RecastLayers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Recast.dir/Source/RecastLayers.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastLayers.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastLayers.cpp

CMakeFiles/Recast.dir/Source/RecastLayers.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastLayers.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastLayers.cpp > CMakeFiles/Recast.dir/Source/RecastLayers.i

CMakeFiles/Recast.dir/Source/RecastLayers.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastLayers.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastLayers.cpp -o CMakeFiles/Recast.dir/Source/RecastLayers.s

CMakeFiles/Recast.dir/Source/RecastMesh.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastMesh.o: Source/RecastMesh.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Recast.dir/Source/RecastMesh.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastMesh.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMesh.cpp

CMakeFiles/Recast.dir/Source/RecastMesh.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastMesh.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMesh.cpp > CMakeFiles/Recast.dir/Source/RecastMesh.i

CMakeFiles/Recast.dir/Source/RecastMesh.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastMesh.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMesh.cpp -o CMakeFiles/Recast.dir/Source/RecastMesh.s

CMakeFiles/Recast.dir/Source/RecastMeshDetail.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastMeshDetail.o: Source/RecastMeshDetail.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Recast.dir/Source/RecastMeshDetail.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastMeshDetail.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMeshDetail.cpp

CMakeFiles/Recast.dir/Source/RecastMeshDetail.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastMeshDetail.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMeshDetail.cpp > CMakeFiles/Recast.dir/Source/RecastMeshDetail.i

CMakeFiles/Recast.dir/Source/RecastMeshDetail.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastMeshDetail.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastMeshDetail.cpp -o CMakeFiles/Recast.dir/Source/RecastMeshDetail.s

CMakeFiles/Recast.dir/Source/RecastRasterization.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastRasterization.o: Source/RecastRasterization.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Recast.dir/Source/RecastRasterization.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastRasterization.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRasterization.cpp

CMakeFiles/Recast.dir/Source/RecastRasterization.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastRasterization.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRasterization.cpp > CMakeFiles/Recast.dir/Source/RecastRasterization.i

CMakeFiles/Recast.dir/Source/RecastRasterization.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastRasterization.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRasterization.cpp -o CMakeFiles/Recast.dir/Source/RecastRasterization.s

CMakeFiles/Recast.dir/Source/RecastRegion.o: CMakeFiles/Recast.dir/flags.make
CMakeFiles/Recast.dir/Source/RecastRegion.o: Source/RecastRegion.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Recast.dir/Source/RecastRegion.o"
	/Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Recast.dir/Source/RecastRegion.o -c /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRegion.cpp

CMakeFiles/Recast.dir/Source/RecastRegion.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Recast.dir/Source/RecastRegion.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRegion.cpp > CMakeFiles/Recast.dir/Source/RecastRegion.i

CMakeFiles/Recast.dir/Source/RecastRegion.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Recast.dir/Source/RecastRegion.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/darkhead/CLionProjects/recastnavigation/Recast/Source/RecastRegion.cpp -o CMakeFiles/Recast.dir/Source/RecastRegion.s

# Object files for target Recast
Recast_OBJECTS = \
"CMakeFiles/Recast.dir/Source/Recast.o" \
"CMakeFiles/Recast.dir/Source/RecastAlloc.o" \
"CMakeFiles/Recast.dir/Source/RecastArea.o" \
"CMakeFiles/Recast.dir/Source/RecastAssert.o" \
"CMakeFiles/Recast.dir/Source/RecastContour.o" \
"CMakeFiles/Recast.dir/Source/RecastFilter.o" \
"CMakeFiles/Recast.dir/Source/RecastLayers.o" \
"CMakeFiles/Recast.dir/Source/RecastMesh.o" \
"CMakeFiles/Recast.dir/Source/RecastMeshDetail.o" \
"CMakeFiles/Recast.dir/Source/RecastRasterization.o" \
"CMakeFiles/Recast.dir/Source/RecastRegion.o"

# External object files for target Recast
Recast_EXTERNAL_OBJECTS =

libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/Recast.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastAlloc.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastArea.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastAssert.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastContour.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastFilter.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastLayers.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastMesh.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastMeshDetail.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastRasterization.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/Source/RecastRegion.o
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/build.make
libRecast.VERSION.dylib: CMakeFiles/Recast.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX shared library libRecast.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Recast.dir/link.txt --verbose=$(VERBOSE)
	$(CMAKE_COMMAND) -E cmake_symlink_library libRecast.VERSION.dylib libRecast.VERSION.dylib libRecast.dylib

libRecast.dylib: libRecast.VERSION.dylib
	@$(CMAKE_COMMAND) -E touch_nocreate libRecast.dylib

# Rule to build all files generated by this target.
CMakeFiles/Recast.dir/build: libRecast.dylib

.PHONY : CMakeFiles/Recast.dir/build

CMakeFiles/Recast.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Recast.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Recast.dir/clean

CMakeFiles/Recast.dir/depend:
	cd /Users/darkhead/CLionProjects/recastnavigation/Recast && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/darkhead/CLionProjects/recastnavigation/Recast /Users/darkhead/CLionProjects/recastnavigation/Recast /Users/darkhead/CLionProjects/recastnavigation/Recast /Users/darkhead/CLionProjects/recastnavigation/Recast /Users/darkhead/CLionProjects/recastnavigation/Recast/CMakeFiles/Recast.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Recast.dir/depend

