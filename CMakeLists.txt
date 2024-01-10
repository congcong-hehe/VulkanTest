cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(Vulkan_Test)
set(CMAKE_CXX_STANDARD 17)

# vulkan
find_package(Vulkan REQUIRED)
link_libraries(${Vulkan_LIBRARY})
include_directories(${Vulkan_INCLUDE_DIRS})

# glm
include_directories(third_party/glm)

# glfw
add_subdirectory(third_party/glfw)
link_libraries(glfw)

# target
add_executable(Draw draw/draw.cpp)

add_executable(DrawIndexed draw_indexed/draw_indexed.cpp)

add_executable(DrawInstanced draw_instanced/draw_instanced.cpp)
