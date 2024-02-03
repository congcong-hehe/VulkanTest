cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

project(Vulkan_Test)
set(CMAKE_CXX_STANDARD 20)

# vulkan
find_package(Vulkan REQUIRED)
link_libraries(${Vulkan_LIBRARY})
include_directories(${Vulkan_INCLUDE_DIRS})

# glm
include_directories(third_party/glm)

# glfw
add_subdirectory(third_party/glfw)
link_libraries(glfw)

# common
include_directories(common)
add_library(Common 
    common/buffer.cpp
    common/graphic_test.cpp
    common/tools.cpp
    common/buffer.cpp
)

# target
add_executable(Draw draw/draw.cpp)
target_link_libraries(Draw Common)

add_executable(DrawIndexed draw_indexed/draw_indexed.cpp)
target_link_libraries(DrawIndexed Common)

add_executable(DrawInstanced draw_instanced/draw_instanced.cpp)
target_link_libraries(DrawInstanced Common)

add_executable(DynamicRendering dynamic_rendering/dynamic_rendering.cpp)
target_link_libraries(DynamicRendering Common)

add_executable(SecondaryBuffer secondary_buffer/secondary_buffer.cpp)
target_link_libraries(SecondaryBuffer Common)

add_executable(DrawIndirect draw_indirect/draw_indirect.cpp)
target_link_libraries(DrawIndirect Common)