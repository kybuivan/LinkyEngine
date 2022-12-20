#pragma once
#include "screen/main/window.h"
class Application {
public:
    Application();
    ~Application();
    virtual void OnUpdate(float deltatime) {};
    virtual void OnImGuiRender(float deltatime) {};
    virtual void OnStart() {};
    virtual void OnEnd() {};

    void Render();
    void ImGuiRenderBegin();
    void ImGuiRenderEnd();
    void Run();

    inline void Close() { m_isActive = false; }

    Window* GetWindow() { return m_Window; }

    static inline Application* Get() { return s_App; }
private:
    float previousTime;
    bool m_isActive;
    Window* m_Window;
private:
    static Application* s_App;
};