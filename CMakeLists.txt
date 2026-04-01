cmake_minimum_required(VERSION 3.28)
project(Snake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(${PROJECT_NAME})

# Compile Options
target_compile_options(${PROJECT_NAME} PRIVATE
  -Wno-c99-designator
)

# pch
target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.h)

# src
add_subdirectory(src)

# Includes
target_include_directories(${PROJECT_NAME} PRIVATE 
  vendor/include
  src
)

# Find Packages
find_package(OpenGL REQUIRED)

# External Libs
target_link_libraries(${PROJECT_NAME} PRIVATE 
  ${CMAKE_SOURCE_DIR}/vendor/lib/glfw3.lib
  OpenGL::GL
)
