cmake_minimum_required(VERSION 3.16)
project(M6_Atividade_Vivencial_Bruna_Chies LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenGL REQUIRED)
find_package(glfw3  REQUIRED)
find_package(GLEW   REQUIRED)
find_package(glm    REQUIRED)

add_executable(tilemap src/main.cpp)

target_include_directories(tilemap PRIVATE src)

target_link_libraries(tilemap PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
    glm::glm
)

if(APPLE)
    target_link_libraries(tilemap PRIVATE
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
    )
endif()

add_custom_command(TARGET tilemap POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:tilemap>/assets
    COMMENT "Copiando assets para o diretório de build"
)
