#include "menu_bar.h"
#include <imgui.h>

void MenuBar::ShowMenu()
{
	if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) {
				
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}