cmake_minimum_required(VERSION 3.29)
project(RedBlackTree_AED)

set(CMAKE_CXX_STANDARD 17)

add_executable(RB-Tree main.cpp)

find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)
if(SFML_FOUND)
    target_link_libraries(RB-Tree PRIVATE sfml-graphics sfml-window sfml-system)
else()
    message("SFML not found")
endif()

# Copiar carpeta assets al directorio de build
add_custom_command(TARGET RB-Tree POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets
    COMMENT "Copiando assets al directorio de build"
)