cmake_minimum_required(VERSION 3.10)
project(StudentuRusiavimas)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(studentai
    main.cpp
    apdorojimas.cpp
    failu_generavimas.cpp
    konteineriu_testavimas.cpp
    nuskaityti.cpp
    studentas.cpp
    util.cpp
    strategijos.cpp
)

target_include_directories(studentai PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(studentai PRIVATE -O2)
endif()