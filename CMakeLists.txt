# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(ImGuiOpenGLExample)

set(CMAKE_CXX_STANDARD 20)
# Add include dir
include_directories("include")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add GLFW
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})

# Add OpenGL
find_package(OpenGL REQUIRED)

# Add Dear ImGui
include_directories("thirdparty/imgui")
# Add ImGui source files
file(GLOB IMGUI_SOURCES "thirdparty/imgui/*.cpp")
file(GLOB IMGUI_OPENGL3_Backend "thirdparty/imgui/backends/imgui_impl_opengl3.cpp")
file(GLOB IMGUI_GLFW_Backend "thirdparty/imgui/backends/imgui_impl_glfw.cpp")

# Add GLM
include_directories("thirdparty/glm")

# Add GLEW
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

# Add executable
add_executable(ImGuiOpenGLExample main.cpp ${SOURCES}
                    ${IMGUI_SOURCES} ${IMGUI_OPENGL3_Backend} ${IMGUI_GLFW_Backend})
target_link_libraries(ImGuiOpenGLExample glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
