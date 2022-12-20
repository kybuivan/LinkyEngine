#pragma once

struct GLFWwindow;

#include <functional>
#include "core/math/color.h"

using EventFunc = std::function<void(int, int)>;

class Window {
public:
    Window();
    ~Window();

    void SetShouldCloseCallback(EventFunc callbackFunction);
    void SetResizeCallback(EventFunc callbackFunction);
    void SetMouseCallback(EventFunc callbackFunction);

    void SetVSync(bool enabled);
    void MakeCurrentContext();
    void SetClearColor(Color color);
    void Update();
    void Close();
    void Clear();

    inline bool IsVSyncEnabled() { return vSyncState; }
    inline GLFWwindow* GetNativeWindow() { return m_Window; }

private:
    bool isActive, vSyncState;
    GLFWwindow* m_Window;
    EventFunc m_CloseEventCallback, m_ResizeEventCallback, m_MouseEventCallback;
    Color m_ClearColor;
};