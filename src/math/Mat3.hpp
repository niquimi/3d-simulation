#pragma once
#include "Vec3.hpp"
#include <cmath>

struct Mat3 {
    float m[3][3];

    // Constructors
    Mat3() {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = 0.0f;
            }
        }
    }

    Mat3(const float arr[3][3]) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                m[i][j] = arr[i][j];
            }
        }
    }

    // ===== Factories =====

    static Mat3 identity() {
        Mat3 r;
        r.m[0][0] = 1, r.m[1][1] = 1, r.m[2][2] = 1;
        return r;
    }

    static Mat3 rotationX(float a) {
        Mat3 r = identity();
        float c = std::cos(a);
        float s = std::sin(a);

        r.m[1][1] = c; r.m[1][2] = -s;
        r.m[2][1] = s; r.m[2][2] = c;
        return r;
    }

    static Mat3 rotationY(float a) {
        Mat3 r = identity();
        float c = std::cos(a);
        float s = std::sin(a);

        r.m[0][0] = c;  r.m[0][2] = s;
        r.m[2][0] = -s; r.m[2][2] = c;
        return r;
    }

    static Mat3 rotationZ(float a) {
        Mat3 r = identity();
        float c = std::cos(a);
        float s = std::sin(a);

        r.m[0][0] = c;  r.m[0][1] = -s;
        r.m[1][0] = s;  r.m[1][1] = c;
        return r;
    }

    static Mat3 scale(float sx, float sy, float sz) {
        Mat3 r;
        r.m[0][0] = sx;
        r.m[1][1] = sy;
        r.m[2][2] = sz;
        return r;
    }

    // ===== Operators =====

    Mat3 operator+( const Mat3& b) const {
        Mat3 r;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                r.m[i][j] = m[i][j] + b.m[i][j];
            }
        }
        return r;
    }

    Mat3 operator*(float scalar) const {
        Mat3 r;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                r.m[i][j] = m[i][j] * scalar;
            }
        }
        return r;
    }

    Mat3& operator*=(const Mat3& b) {
        *this = *this * b;
        return *this;
    }

    Mat3 operator*(const Mat3& b) const {
        Mat3 r;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                r.m[i][j] = m[i][0] * b.m[0][j] + m[i][1] * b.m[1][j] + m[i][2] * b.m[2][j];
            }
        }
        return r;
    }

    Vec3 operator*(const Vec3& v) const {
        return Vec3(
            m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
            m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
            m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
        );
    }

    // ===== Math functions =====

    Mat3 transpose() const {
        Mat3 r;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                r.m[i][j] = m[j][i];
            }
        }
        return r;
    }

    float determinant() const {
        return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
               m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
               m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
    }

    Mat3 inverse() const {
        Mat3 r;
        float det = determinant();

        if (std::abs(det) < 1e-8f) return Mat3(); // No inverse if determinant is zero

        float invDet = 1.0f / det;

        r.m[0][0] =  (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
        r.m[0][1] = -(m[0][1] * m[2][2] - m[0][2] * m[2][1]) * invDet;
        r.m[0][2] =  (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;

        r.m[1][0] = -(m[1][0] * m[2][2] - m[1][2] * m[2][0]) * invDet;
        r.m[1][1] =  (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
        r.m[1][2] = -(m[0][0] * m[1][2] - m[0][2] * m[1][0]) * invDet;

        r.m[2][0] =  (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
        r.m[2][1] = -(m[0][0] * m[2][1] - m[0][1] * m[2][0]) * invDet;
        r.m[2][2] =  (m[0][0] * m[1][1] - m[0][1] * m[1][0]) * invDet;

        return r;
    }

    // ===== Comparison =====

    static constexpr float EPSILON = 1e-6f;

    bool operator==(const Mat3& other) const {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (std::abs(m[i][j] - other.m[i][j]) >= EPSILON) {
                    return false;
                }
            }
        }
        return true;
    }

    bool operator!=(const Mat3& other) const {
        return !(*this == other);
    }
};
