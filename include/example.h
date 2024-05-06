#pragma once
#include <math.h>
#include <imgui.h>

void showExampleFrame(bool *myToolActive, float (*color)[4]) {
  ImGui::Begin("My First Tool", myToolActive, ImGuiWindowFlags_MenuBar);
  if (ImGui::BeginMenuBar())
  {       
      if (ImGui::BeginMenu("File"))
      {
          if (ImGui::MenuItem("Open..", "Ctrl+O")) { /* Do stuff */ }
          if (ImGui::MenuItem("Save", "Ctrl+S"))   { /* Do stuff */ }
          if (ImGui::MenuItem("Close", "Ctrl+W"))  { *myToolActive = false; }
          ImGui::EndMenu();
      }
      ImGui::EndMenuBar();
  }

  // Edit a color stored as 4 floats
  ImGui::ColorEdit4("Color", *color);

  // Generate samples and plot them
  float samples[100];
  for (int n = 0; n < 100; n++)
      samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
  ImGui::PlotLines("Samples", samples, 100);

  // Display contents in a scrolling region
  ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
  ImGui::BeginChild("Scrolling");
  for (int n = 0; n < 50; n++)
      ImGui::Text("%04d: Some text", n);
  ImGui::EndChild();
  ImGui::End();
}