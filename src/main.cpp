#include <Windows.h>
#include "physics/PhysicsWorld.hpp"
#include "renderer/Camera.hpp"
#include "platform/Window.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {

    PhysicsWorld world(5.0f);

    Camera cam;
    cam.position = Vec3(3, 2, -20);
    cam.target = Vec3(0, 0, 0);
    cam.focalPoint = 500.0f;

    Window window(hInstance, &world, &cam);
    window.run();

    return 0;
}