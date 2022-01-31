cmake_minimum_required(VERSION 3.18.2)
project(CPL-Project-2-211850016)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)

add_executable(Sakiyary_EmojiBattle "Sakiyary_EmojiBattle.c")
target_link_libraries(Sakiyary_EmojiBattle SDL2)
target_link_libraries(Sakiyary_EmojiBattle SDL2_image)
target_link_libraries(Sakiyary_EmojiBattle SDL2_ttf)
target_link_libraries(Sakiyary_EmojiBattle SDL2_mixer)