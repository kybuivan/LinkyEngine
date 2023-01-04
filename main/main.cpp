#include "screen/resources/texture.h"
#include "screen/resources/shader.h"
#include "screen/resources/shader_m.h"
//#include "screen/resources/mesh.h"
#include "screen/main/window.h"
#include "screen/gui/menu_bar.h"
#include "screen/main/application.h"
#include "editor/editor_node.h"
#include "editor/editor_dock.h"
#include "screen/3d/camera_3d.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#define IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#include "imgui_impl_opengl3.h"
#include <iostream>

static Application* myApp;
static Camera3D camera(glm::vec3(0.0f, 0.0f, 3.0f));
static bool show_demo_window = true;
static bool show_another_window = false;
static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
Ref<Texture2D> image;
// build and compile our shader zprogram
// ------------------------------------
static glm::vec2 prevMousePos(0,0);
bool firstMouse = true;
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;
// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// cube VAO
unsigned int cubeVAO, cubeVBO;
unsigned int planeVAO, planeVBO;
unsigned int quadVAO, quadVBO;
unsigned int framebuffer;
unsigned int rbo;
 unsigned int textureColorbuffer;
float cubeVertices[] = {
    // positions          // texture Coords
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
float planeVertices[] = {
    // positions          // texture Coords 
    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

    5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
    -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
    5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};
float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
    1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
    1.0f, -1.0f,  1.0f, 0.0f,
    1.0f,  1.0f,  1.0f, 1.0f
};

static void MouseMoveCallback(float x, float y) {
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);
    if (firstMouse)
    {
        prevMousePos.x = xpos;
        prevMousePos.y = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - prevMousePos.x;
    float yoffset = prevMousePos.y - ypos; // reversed since y-coordinates go from bottom to top

    prevMousePos.x = xpos;
    prevMousePos.y = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void KeyboarInput(ImGuiIO io)
{
    float cameraSpeed = 0.1;
    if (ImGui::IsKeyPressed(ImGuiKey_A))
        camera.ProcessKeyboard(Camera3D_Movement::LEFT,cameraSpeed);
    if (ImGui::IsKeyPressed(ImGuiKey_W))
        camera.ProcessKeyboard(Camera3D_Movement::FORWARD,cameraSpeed);
    if (ImGui::IsKeyPressed(ImGuiKey_S)) // r Key
        camera.ProcessKeyboard(Camera3D_Movement::BACKWARD,cameraSpeed);
    if (ImGui::IsKeyPressed(ImGuiKey_D)) // r Key
        camera.ProcessKeyboard(Camera3D_Movement::RIGHT,cameraSpeed);
}

void ShowMainScene() {
    auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
    auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
    auto viewportOffset = ImGui::GetWindowPos();

    ImGui::Begin("Viewport");
    {
        ImGui::BeginChild("MainRender");
        if (ImGui::IsWindowHovered()) {
            ImGuiIO io = ImGui::GetIO();
            MouseMoveCallback(io.MousePos.x, io.MousePos.y);
            camera.ProcessMouseScroll(static_cast<float>(io.MouseWheel));
            KeyboarInput(io);
            if (ImGui::GetIO().MouseDown[1]) {
                
            }
        }

        ImVec2 wsize = ImGui::GetWindowSize();
        ImGui::Image((void*)(intptr_t)textureColorbuffer, wsize);
        ImGui::EndChild();
    }
    ImGui::End();
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
unsigned int cubeTexture, floorTexture ;

class MyApp : public Application 
{
public:
    virtual void OnUpdate(float deltatime) override
    {
        // per-frame time logic
        // --------------------
        //float currentFrame = static_cast<float>(glfwGetTime());
        //deltaTime = currentFrame - lastFrame;
        //lastFrame = currentFrame;

        // ------
        // bind to framebuffer and draw scene as we normally would to color texture 
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

        // make sure we clear the framebuffer's content
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);
        // cubes
        glBindVertexArray(cubeVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, image->GetRendererID());
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        // floor
        glBindVertexArray(planeVAO);
        glBindTexture(GL_TEXTURE_2D, image->GetRendererID());
        shader.setMat4("model", glm::mat4(1.0f));
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST); // disable depth test so screen-space quad isn't discarded due to depth test.
        // clear all relevant buffers
        //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        //glClear(GL_COLOR_BUFFER_BIT);

        screenShader.use();
        glBindVertexArray(quadVAO);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// use the color attachment texture as the texture of the quad plane
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    virtual void OnImGuiRender(float deltatime) override
    {
        EditorDock::OnBeforeImGuiRender();
        MenuBar::ShowMenu();
        ShowMainScene();
        EditorNode::NodeEditorShow();
        ImGui::End();
    }

    virtual void OnStart() override
    {
        Log("Started Up App!");
        GetWindow()->SetShouldCloseCallback(OnAppClose);
        //  glfwSetFramebufferSizeCallback(GetWindow()->GetNativeWindow(), [] (GLFWwindow* window, int w, int h){
        //         glViewport(0, 0, w, h);
        //         glfwSwapBuffers(window);
        //     });
        GetWindow()->SetClearColor({0.1f, 0.1f, 0.1f});
        image = Texture2D::Create("./icon.png");
        shader = Shader("assets/texture.vert", "assets/texture.frag");
        screenShader = Shader("assets/texture.vert", "assets/texture.frag");

        // cube VAO
        glGenVertexArrays(1, &cubeVAO);
        glGenBuffers(1, &cubeVBO);
        glBindVertexArray(cubeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // plane VAO
        glGenVertexArrays(1, &planeVAO);
        glGenBuffers(1, &planeVBO);
        glBindVertexArray(planeVAO);
        glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        // screen quad VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        // shader configuration
        // --------------------
        shader.use();
        shader.setInt("texture1", 0);

        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        // framebuffer configuration
        // -------------------------
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
        // create a color attachment texture
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        // create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
        // glGenRenderbuffers(1, &rbo);
        // glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600); // use a single renderbuffer object for both a depth AND stencil buffer.
        // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // now actually attach it
        // // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
        // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        //     std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
	    EditorNode::NodeEditorInitialize();

    }

    void OnEnd() 
    {
        EditorNode::NodeEditorShutdown();
    }
private:
    Shader shader;
    Shader screenShader;
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