#pragma once
#include "application.h"

extern Application* CreateApplication();

int main()
{
    Application* app = CreateApplication();
    app->Run();
    delete app;
}