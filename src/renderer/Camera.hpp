#pragma once
#include "../math/Vec3.hpp"
#include <Windows.h>

struct Camera {
    Vec3 position;
    Vec3 target;

    Vec3 forward;
    Vec3 up;
    Vec3 right;

    float focalPoint;

    void updateBasis();

    POINT projectPoint(const Vec3& worldPoint, int screenW, int screenH) const;
};