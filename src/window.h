#pragma once

struct GLFWwindow;

#include <functional>

using EventFunc = std::function<void(int, int)>;

struct ClearColor {
    ClearColor() {
        r = g = b = 0;
    }

    ClearColor(float r, float g, float b)
        : r(r), g(g), b(b) {}

    float r;
    float g;
    float b;
};

class Window {
public:
    Window();
    ~Window();

    void SetShouldCloseCallback(EventFunc callbackFunction);
    void SetResizeCallback(EventFunc callbackFunction);
    void SetMouseCallback(EventFunc callbackFunction);

    void SetVSync(bool enabled);
    void MakeCurrentContext();
    void SetClearColor(ClearColor color);
    void Update();
    void Close();
    void Clear();

    inline bool IsVSyncEnabled() { return vSyncState; }
    inline GLFWwindow* GetNativeWindow() { return m_Window; }

private:
    bool isActive, vSyncState;
    GLFWwindow* m_Window;
    EventFunc m_CloseEventCallback, m_ResizeEventCallback, m_MouseEventCallback;
    ClearColor m_ClearColor;
};