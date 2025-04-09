cmake_minimum_required(VERSION 3.8)

project("necrowarp")

find_package(bleak REQUIRED)

add_executable(necrowarp "src/program.cpp")

target_include_directories(necrowarp PRIVATE inc inl)

target_link_libraries(necrowarp INTERFACE bleak::bleak)
