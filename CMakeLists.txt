cmake_minimum_required(VERSION 3.22)
project(dayz_external_cpp)

set(CMAKE_CXX_STANDARD 20)

# Dayz main client
add_executable(client #WIN32
        main.cpp
        memory.cpp
        math.h
        world.h
        math.cpp
        world.cpp
        camera.h
        camera.cpp
        entity.h
        entity.cpp
        utility.h
        utility.cpp
        render.h
        render.cpp
        network.h 
        network.cpp 
        config.h 
        config.cpp)

target_include_directories(client
        PUBLIC
            include)

target_link_libraries(client
        PUBLIC
            d3d11.lib)

add_subdirectory(imgui)