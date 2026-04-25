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
    };

    Mat3(float arr[3][3]) {
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
        r.m[2][1] = s; r.m[2][2] = -c;
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

};
