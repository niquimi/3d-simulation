#include "Camera.hpp"
#include <algorithm>

constexpr float PI = 3.14159265359f;

void Camera::updateBasis() {
    forward = (target - position).normalized();

    Vec3 worldUp = Vec3::up();

    right = forward.cross(worldUp).normalized();
    up = right.cross(forward);
};

void Camera::updatePosition() {
    position.x = target.x + distance * cos(pitch) * sin(yaw);
    position.y = target.y + distance * sin(pitch);
    position.z = target.z + distance * cos(pitch) * cos(yaw);

    updateBasis();
}

void Camera::orbit(float dx, float dy) {
    yaw -= dx * sensitivity;
    pitch += dy * sensitivity;

    pitch = std::clamp(pitch, -PI/2 + 0.05f, PI/2 - 0.05f);

    updatePosition();
}

void Camera::zoom(float delta) {
    distance -= delta * zoomSpeed;
    distance = std::clamp(distance, 1.0f, 20.0f);

    updatePosition();
}

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