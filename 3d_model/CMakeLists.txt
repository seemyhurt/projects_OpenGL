cmake_minimum_required(VERSION 3.14)

project(3d_model LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_SOURCES
    3d_model.cpp
)

set(PROJECT_RESOURCES
    fragment.txt
    vertex.txt
    model_loading_1.txt
    model_loading_2.txt
)

foreach(RESOURCE ${PROJECT_RESOURCES})
    file(COPY ${CMAKE_CURRENT_LIST_DIR}/proto/${RESOURCE} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/)
    install(FILES ${CMAKE_CURRENT_LIST_DIR}/proto/${RESOURCE}
        DESTINATION ${CMAKE_INSTALL_BINDIR}/${PROJECT_NAME}
    )
endforeach()

file(COPY ${CMAKE_CURRENT_LIST_DIR}/proto/models DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}/)

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES} 
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    utils
    SOIL
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    PROJECT_NAME="${PROJECT_NAME}"
)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

