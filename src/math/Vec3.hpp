#include <math.h>

struct Vec3
{
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {};

    Vec3(float x, float y, float z) : x(x), y(y), z(z) {};

    Vec3 operator+(const Vec3& other) const {
        return Vec3(x + other.x, y + other.y, z + other.z);
    };

    Vec3 operator-(const Vec3& other) const {
        return Vec3(x - other.x, y - other.y, z - other.z);
    };

    Vec3 operator*(const Vec3& other) const {
        return Vec3(x * other.x, y * other.y, z * other.z);
    };

    Vec3 operator/(const Vec3& other) const {
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            return Vec3(x, y, z);
        }
        return Vec3(x / other.x, y / other.y, z / other.z);
    }

};
