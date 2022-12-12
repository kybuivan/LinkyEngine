#include <stdio.h>
#include <iostream>

#include "window.h"
#include "application.h"
#include "entry_point.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/gl.h>
#include <GLFW\glfw3.h> // Will drag system OpenGL headers

static Application* myApp;
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

void ShowDemoWindow()
{
    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    myApp->GetWindow()->SetClearColor(ClearColor(clear_color.x, clear_color.y, clear_color.z));
    myApp->GetWindow()->Clear();
}

static void Log(const char* log)
{
    std::cout << log << std::endl;
};

void OnAppClose(int x, int y)
{
    Log("Close App");
    myApp->Close();
}

class MyApp : public Application 
{
public:
    virtual void OnUpdate(float deltatime) override
    {
    }

    virtual void OnImGuiRender(float deltatime) override
    {
        ShowDemoWindow();
    }

    virtual void OnStart() override
    {
        Log("Started Up App!");
        GetWindow()->SetShouldCloseCallback(OnAppClose);
        glfwSetFramebufferSizeCallback(GetWindow()->GetNativeWindow(), [] (GLFWwindow* window, int w, int h){
            //glViewport(0, 0, w, h);
            glfwSwapBuffers(window);
            });
        GetWindow()->SetClearColor({0.1f, 0.1f, 0.1f});
    }

    void OnEnd() 
    {
    }
};

Application* CreateApplication()
{
    myApp = new MyApp();
    return myApp;
}