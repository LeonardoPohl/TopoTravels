// main.cpp

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "util/helper/GLFWHelper.h"
#include "util/helper/ImGuiHelper.h"
#include "util/WindowInfo.h"

#include "example.h"

int main()
{
    // Initialize GLFW
    if (!glfwInit())
        return -1;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui OpenGL Example", NULL, NULL);

    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    // Setup Dear ImGui context
    ImGuiHelper::initImGui(window);

    bool myToolActive = true;
    float color[4]{0.0f, 0.0f, 0.0f, 1.00f};
    
    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        // Poll and handle events
        glfwPollEvents();

        // Start the ImGui frame
        ImGuiHelper::beginFrame();

        // Your ImGui widgets/code goes here
        showExampleFrame(&myToolActive, &color);

        // Rendering
        glClearColor(color[0], color[1], color[2], color[3]);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGuiHelper::endFrame();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGuiHelper::terminateImGui();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
