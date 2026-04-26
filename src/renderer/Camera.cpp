#include "Camera.hpp"

void Camera::updateBasis() {
    forward = (target - position).normalized();

    Vec3 worldUp = Vec3::up();

    right = forward.cross(worldUp).normalized();
    up = right.cross(forward);
};

POINT Camera::projectPoint(const Vec3& worldPoint, int screenW, int screenH) const{
    
        Vec3 rel = worldPoint - position;

        float x = rel.dot(right);
        float y = rel.dot(up);
        float z = rel.dot(forward);

        if (z <= 0.001f) return POINT{-1, -1};

        float screenX = (x * focalPoint) / z;
        float screenY = (y * focalPoint) / z;

        screenX += screenW * 0.5f;
        screenY = screenH * 0.5f - screenY;

        return POINT{static_cast<LONG>(screenX), static_cast<LONG>(screenY)};
}