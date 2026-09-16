cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0048 NEW)
project(DarwinianSimulation CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
)
FetchContent_MakeAvailable(raylib)

add_executable(darwinian_sim main.cpp)
target_link_libraries(darwinian_sim PRIVATE raylib)