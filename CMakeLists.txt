cmake_minimum_required(VERSION 3.14)

project(OpenGL_projects LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/glew/include)
include_directories(${CMAKE_SOURCE_DIR}/3rd_party/glm)


set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    message("CCACHE Found, using it for build")
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
else()
    message("CCACHE not found")
endif(CCACHE_FOUND)

set(PROJECT_IMAGES
    awesomeface.png
    container.jpg
    container.png
    container_specular.png
    texture.jpg
)

foreach(IMAGE ${PROJECT_IMAGES})
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/images/${IMAGE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/images/)
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/images/${IMAGE}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/images
    )
endforeach()

add_subdirectory(3rd_party)
add_subdirectory(utils)
add_subdirectory(color)
add_subdirectory(changecolor)
add_subdirectory(transform)
add_subdirectory(camera)
add_subdirectory(first3d)
add_subdirectory(texture_triangle)
add_subdirectory(shaders)
add_subdirectory(light_source)
add_subdirectory(materials)
add_subdirectory(many_light_source)
add_subdirectory(texture_color)
add_subdirectory(3d_model)
add_subdirectory(3d_object)
add_subdirectory(adaptive_light)
