#pragma once
#include "../math/Vec3.hpp"
#include <Windows.h>

struct Camera {
    Vec3 position;
    Vec3 target;

    Vec3 forward;
    Vec3 up;
    Vec3 right;

    float yaw;
    float pitch;
    float distance;

    float focalPoint;
    float sensitivity = 0.005f;
    float zoomSpeed = 0.005f;

    void updateBasis();
    void updatePosition();
    void orbit(float dx, float dy);
    void zoom(float delta);

    POINT projectPoint(const Vec3& worldPoint, int screenW, int screenH) const;
};