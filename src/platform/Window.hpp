#pragma once
#include <Windows.h>
#include "../physics/PhysicsWorld.hpp"
#include "../renderer/Camera.hpp"

struct Window {
    HWND hwnd;
    PhysicsWorld* world;
    Camera* cam;
    bool running;

    bool mouseDown;
    POINT lastMousePos;

    Window(HINSTANCE hInstance, PhysicsWorld* world, Camera* cam);

    void run();

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};
