cmake_minimum_required(VERSION 3.21)

#-----------------------------------------------------------------------------------------

project(tatlin)

#-----------------------------------------------------------------------------------------

set(TESTING_ENABLED ON CACHE BOOL [FORCE])

#-----------------------------------------------------------------------------------------

# set(CMAKE_BUILD_TYPE "Release")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS  -fsanitize=address,leak)
    message("Debug_version")
endif ()

#-----------------------------------------------------------------------------------------

add_subdirectory(config/)
add_subdirectory(debug_utils/)
add_subdirectory(magnetic_tape/)

if (TESTING_ENABLED)
    add_subdirectory(tests/)
    message("Test are enabled")
endif()

unset(TESTING_ENABLED CACHE)


