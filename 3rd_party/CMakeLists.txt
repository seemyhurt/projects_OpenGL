cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/glew/include)
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/glm)

add_subdirectory(soil)
add_subdirectory(glm)
add_subdirectory(glfw)
add_subdirectory(glew/build/cmake)
add_subdirectory(assimp)