# CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
project(TopoTravels)

set(CMAKE_CXX_STANDARD 20)

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

# Add Boost
find_package(Boost REQUIRED)

# Add Curl
find_package(CURL REQUIRED)

# Add include dir
include_directories("include")
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Copy resources
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
make_directory(${CMAKE_CURRENT_BINARY_DIR}/resources/raw)

# Add executable
add_executable(${PROJECT_NAME} main.cpp ${SOURCES}
                    ${IMGUI_SOURCES} ${IMGUI_OPENGL3_Backend} ${IMGUI_GLFW_Backend})
target_link_libraries(${PROJECT_NAME} glfw ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} ${CURL_LIBRARIES})