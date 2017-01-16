#
#   \file   CMakeLists.txt
#   \brief  Instructions for generating build files for the Nelobin Library.
#
#   This file comes heavily commented, so you can learn how to write out your
#   own CMakeLists.txt files for use in your own projects.
#

# This should be the first line in your CMakeLists.txt. It defines the
# minimum version of the CMake build system required to use this file.
cmake_minimum_required (VERSION 3.1)

# Project Introduction
#
# This will define our project's name, as well as its major version,
# minor version, and patch level.
project (Nelobin VERSION 1.0.0)

# At this point, if you were using dependencies in your project, you would let
# CMake know where to find the necessary find modules to locate those dependencies,
# using the 'find_package' command, like so:
#
# set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules/)
# find_package (Name_Of_Required_Dependency REQUIRED)
# find_package (Name_Of_Optional_Dependency)
#
# if (Name_Of_Optional_Dependency_FOUND)
#   Do some library configuration logic here...
# endif ()

# Use the 'set' command to set custom variables, like the list of header files
# and source files. The first argument is the variable name, and the rest comprise
# the variable's value.
set (
    HEADER_FILES
        Include/Nelobin/Export.hpp
        Include/Nelobin/Types.hpp
        Include/Nelobin/Error.hpp
        Include/Nelobin/Order.hpp
        Include/Nelobin/Buffer.hpp
        Include/Nelobin/EntryBase.hpp
        Include/Nelobin/Entry.hpp
        Include/Nelobin/Key.hpp
        Include/Nelobin/Registry.hpp
)

set (
    SOURCE_FILES
        Source/Nelobin/Types.cpp
        Source/Nelobin/Error.cpp
        Source/Nelobin/Order.cpp
        Source/Nelobin/Buffer.cpp
        Source/Nelobin/EntryBase.cpp
        Source/Nelobin/Entry.cpp
        Source/Nelobin/Key.cpp
        Source/Nelobin/Registry.cpp
)

# Check to see if we wish to build this project in debug mode, or a release mode.
# Perform some build-type-dependent configurations appropriately.
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    # If so, make sure we append '-d' to the final output library.
    set (OUTPUT_LIBRARY ${PROJECT_NAME}-d)

    # Also add a macro definition to indicate building in debug mode.
    # This line may seem like it is compiler-specific, but CMake should actually
    # adapt this macro definition for other compilers, like MSVC.
    add_definitions (-DNELOBIN_DEBUG_MODE)
else ()
    # If the build type is set to anything else, it can be assumed that we
    # are building in some sort of release mode.
    #
    # In that case, set the name of the output library to the project name.
    set (OUTPUT_LIBRARY ${PROJECT_NAME})
endif ()

# Add another macro definition to indicate that the functions and classes in
# this library are being exported out to a shared library (.so on Linux,
# .dll on Windows).
add_definitions (-DNELOBIN_BUILDING)

# Let CMake know where to find the library's include files for building.
include_directories (Include)

# Let CMake know that we are building a shared library with the given output
# name and the list of source files we made out earlier.
add_library (${OUTPUT_LIBRARY} SHARED ${SOURCE_FILES})

# Let CMake know where the library and include files should be installed.
install (
    TARGETS ${OUTPUT_LIBRARY}
    LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
)

install (
    FILES ${HEADER_FILES}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/Nelobin
)
