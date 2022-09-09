cmake_minimum_required(VERSION 3.23.3)

project(example)

set(imgui_sources
      imgui/source/imgui_demo.cpp
      imgui/source/imgui_draw.cpp
      imgui/source/imgui_impl_glfw.cpp
      imgui/source/imgui_impl_opengl3.cpp
      imgui/source/imgui_tables.cpp
      imgui/source/imgui_widgets.cpp
      imgui/source/imgui.cpp
)


find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

add_executable(example example.cpp glad/source/glad.c ${imgui_sources})
target_include_directories(example PRIVATE glad/include imgui/include)
target_link_libraries(example glfw glm)
