#include "screen/resources/texture.h"
#include "screen/main/window.h"
#include "application.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

static Application* myApp;
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
Ref<Texture2D> image;

void ShowMainScene() {
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();

    ImGui::Begin("Viewport");
    {
        ImGui::BeginChild("MainRender");
        if (ImGui::IsWindowHovered()) {
            ImGuiIO io = ImGui::GetIO();
            //MouseMoveCallback(io.MousePos.x, io.MousePos.y);
            //MouseScrollCallback(io.MouseWheel);
            //button1 = io.MouseDown[0];
            //button2 = io.MouseDown[2];
            //button3 = io.MouseDown[1];

            if (ImGui::GetIO().MouseDown[1]) {
                //CameraPosition[0] += -io.MouseDelta.x * 0.005f;
                //CameraPosition[1] += io.MouseDelta.y * 0.005f;
            }

        }
        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void*)(intptr_t)image->GetRendererID(), wsize);
        ImGui::EndChild();
    }
    ImGui::End();
}

void ShowMenu()
{
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) {
                myApp->Close();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
}

void OnBeforeImGuiRender() {

    static bool dockspaceOpen = true;
    static bool opt_fullscreen_persistant = true;
    bool opt_fullscreen = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.1f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();
    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }
    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    ShowMenu();
}

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

    myApp->GetWindow()->SetClearColor(Color(clear_color.x, clear_color.y, clear_color.z));
    myApp->GetWindow()->Clear();
}

void Log(const char* log)
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
        OnBeforeImGuiRender();
        ShowMainScene();
        ShowDemoWindow();
    }

    virtual void OnStart() override
    {
        Log("Started Up App!");
        GetWindow()->SetShouldCloseCallback(OnAppClose);
        // glfwSetFramebufferSizeCallback(GetWindow()->GetNativeWindow(), [] (GLFWwindow* window, int w, int h){
        //     //glViewport(0, 0, w, h);
        //     glfwSwapBuffers(window);
        //     });
        GetWindow()->SetClearColor({0.1f, 0.1f, 0.1f});
        image = Texture2D::Create("./icon.png");
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

int main()
{
    Application* app = CreateApplication();
    app->Run();
    delete app;
}