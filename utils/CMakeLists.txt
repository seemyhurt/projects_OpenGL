cmake_minimum_required(VERSION 3.14)
project(utils LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(PROJECT_SOURCES
)

set(PROJECT_HEADERS
    model.h
    camera.h
    shader.h
)

add_library(${PROJECT_NAME}
    INTERFACE
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    mesh.h

)

target_include_directories(${PROJECT_NAME} INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/3rd_party/glm/
    ${CMAKE_SOURCE_DIR}/3rd_party/glew/include
)

target_link_libraries(${PROJECT_NAME} INTERFACE
    glfw
    glew
    SOIL
    assimp
)
