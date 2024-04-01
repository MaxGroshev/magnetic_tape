cmake_minimum_required(VERSION 3.22)

# -----------------------------------------------------------------------------------------

project(tatlin)

# -----------------------------------------------------------------------------------------

find_package(Boost 1.40 COMPONENTS program_options REQUIRED)

# -----------------------------------------------------------------------------------------

# add_subdirectory(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

foreach(SRC ${SRC_LIST})
        # target_compile_features(${SRC} PUBLIC cxx_std_20)
        # target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/${SRC})
        target_include_directories(${SRC} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endforeach()

# -----------------------------------------------------------------------------------------

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20 )
target_link_libraries(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} debug_utils Boost::program_options)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


