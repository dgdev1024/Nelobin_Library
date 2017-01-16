#
#   \file   FindNelobin.cmake
#   \brief  Contains instructions for finding the Nelobin library in Cmake.
#
#   If the Nelobin library is found, the following CMake variables will be
#   defined:
#
#   - NELOBIN_FOUND
#       Indicates that the Nelobin library was found.
#   - NELOBIN_INCLUDES
#       Indicates where the Nelobin Library's include files were found.
#   - NELOBIN_LIBRARY
#       Indicates where the Nelobin Library itself was found.
#
#   This file comes with comments that should get you started on how to write
#   your own CMake find modules. If you have any tips on how to better the
#   process, feel free to share them.
#

# First, check to see if we are generating build files for debug mode or
# release mode, so we'll know just which library files to look for.
#
# Store those library names in the variable 'NELOBIN_LIBRARY_NAMES' using 'set'.
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    set (NELOBIN_LIBRARY_NAMES Nelobin-d libNelobin-d)
else ()
    set (NELOBIN_LIBRARY_NAMES Nelobin libNelobin)
endif ()

# First, find the Nelobin include files.
#
# The 'NAMES' parameter tells CMake which specific header file(s) to look for,
# using a path relative to paths listed in the 'PATHS' parameter (see later).
#
# The 'HINTS' and 'PATHS' parameters inform CMake of the paths in which the
# files could be found. The 'ENV' keyword tells CMake to also check paths found
# in a given system environment variable.
#
# If Nelobin is not found in the given paths, or said paths do not exist on your
# machine, additional paths for the includes (and for the library, see later)
# can be specified via the variables 'NELOBIN_ADDITIONAL_INCLUDE_DIRS' and
# 'NELOBIN_ADDITIONAL_LIBRARY_DIRS', using the CMake GUI or the -D switch
# in the command line.
find_path (
    NELOBIN_INCLUDES
    NAMES
        Nelobin/Registry.hpp
    HINTS
        ENV NELOBIN_DIR
    PATHS
        /usr/include
        /usr/local/include
        ${NELOBIN_ADDITIONAL_INCLUDE_DIRS}
)

# Now find out where the Nelobin Library itself is located. This is similar
# to when we found the include files in the 'find_path' call above.
find_library (
    NELOBIN_LIBRARY
    NAMES
        ${NELOBIN_LIBRARY_NAMES}
    HINTS
        ENV NELOBIN_DIR
    PATHS
        /usr/lib
        /usr/local/lib
        ${NELOBIN_ADDITIONAL_LIBRARY_DIRS}
)

# Now check to see if both the include files and library files were found.
if (NELOBIN_INCLUDES AND NELOBIN_LIBRARY)
    set (NELOBIN_FOUND true)

    # If the library was found, let the user know where.
    message (STATUS "Found the Nelobin library.")
    message (STATUS "   Includes: ${NELOBIN_INCLUDES}")
    message (STATUS "   Library: ${NELOBIN_LIBRARY}")
else ()
    set (NELOBIN_FOUND false)

    # If the library was not found, let the user know that they can set additional
    # paths to find the library in question.
    message (STATUS "Could not find the Nelobin library.")
    message (STATUS "Additional include paths can be specified with NELOBIN_ADDITIONAL_INCLUDE_DIRS.")
    message (STATUS "Additional library paths can be specified with NELOBIN_ADDITIONAL_LIBRARY_DIRS.")
    message (STATUS "   Includes found: ${NELOBIN_INCLUDES}")
    message (STATUS "   Libraries found: ${NELOBIN_LIBRARY}")

    # If the build requires this library, then throw an error.
    if (Nelobin_FIND_REQUIRED)
        message (FATAL_ERROR "The Nelobin library is REQUIRED. Unable to continue!")
    endif (Nelobin_FIND_REQUIRED)
endif ()

#
# To use this find module in your project, place it in a special folder, like
# "${CMAKE_SOURCE_DIR}/cmake/modules/", where ${CMAKE_SOURCE_DIR} is the directory
# containing your project's top-level "CMakeLists.txt".
#
# In the project's top-level "CMakeLists.txt" file, just after introducing your
# project with the 'project' command, make sure you let CMake know where to look
# for this find module and others by 'set'ting the 'CMAKE_MODULE_PATH' variable
# to the directory in question, like the aforementioned "${CMAKE_SOURCE_DIR}/cmake/modules/".
#
