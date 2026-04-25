#pragma once
#include <cmath>

struct Vec3 {
    float x, y, z;

    // Constructors
    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // ===== Assignment operators =====
    Vec3& operator+=(const Vec3& other) {
        x += other.x; y += other.y; z += other.z;
        return *this;
    }

    Vec3& operator-=(const Vec3& other) {
        x -= other.x; y -= other.y; z -= other.z;
        return *this;
    }

    Vec3& operator*=(float scalar) {
        x *= scalar; y *= scalar; z *= scalar;
        return *this;
    }

    Vec3& operator/=(float scalar) {
        if (std::abs(scalar) < 1e-8f) return *this;
        x /= scalar; y /= scalar; z /= scalar;
        return *this;
    }

    // ===== Binary operators =====
    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    }

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    }

    Vec3 operator*(float scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    Vec3 operator/(float scalar) const {
        if (std::abs(scalar) < 1e-8f) return Vec3(0,0,0);
        return Vec3(x / scalar, y / scalar, z / scalar);
    }

    // Negation
    Vec3 operator-() const {
        return Vec3(-x, -y, -z);
    }

    // ===== Math functions (métodos de instancia) =====
    float dot(const Vec3& other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    Vec3 cross(const Vec3& other) const {
        return Vec3(
            y * other.z - z * other.y,
            z * other.x - x * other.z,
            x * other.y - y * other.x
        );
    }

    float lengthSq() const {
        return dot(*this);
    }

    float length() const {
        return std::sqrt(lengthSq());
    }

    Vec3 normalized() const {
        float len = length();
        if (len < 1e-8f) return Vec3(0,0,0);
        return *this / len;
    }

    static constexpr float EPSILON = 1e-6f;

    // ===== Comparación =====
    bool operator==(const Vec3& other) const {
        return std::abs(x - other.x) < EPSILON &&
            std::abs(y - other.y) < EPSILON &&
            std::abs(z - other.z) < EPSILON;
    }

    bool operator!=(const Vec3& other) const {
        return !(*this == other);
    }

    static Vec3 zero() {
        return Vec3(0.0f, 0.0f, 0.0f);
    }

    static Vec3 up() {
        return Vec3(0.0f, 1.0f, 0.0f);
    }

    static Vec3 forward() {
        return Vec3(0.0f, 0.0f, 1.0f);
    }
};

// ===== Funciones externas =====

// scalar * Vec3 (conmutativo)
inline Vec3 operator*(float scalar, const Vec3& v) {
    return v * scalar;
}

// reflect
inline Vec3 reflect(const Vec3& v, const Vec3& normal) {
    return v - 2.0f * v.dot(normal) * normal;
}

// lerp
inline Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
    return a + (b - a) * t;
}