cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(tatlin)

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])

#-----------------------------------------------------------------------------------------

set(CMAKE_BUILD_TYPE "Debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address,leak)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------


# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
add_subdirectory(config/)
add_subdirectory(debug_utils/)
add_subdirectory(magnetic_tape/)

if (TESTING_ENABLED)
    # add_subdirectory(tests/)
endif()

# unset(TESTING_ENABLED CACHE)


