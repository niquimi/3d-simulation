#include "Collision.hpp"

CollisionManifold sphereVsPlane(const RigidBody& sphere, const Plane& plane) {
    CollisionManifold m;
    m.hit = false;
    m.penetration = 0.f;
    m.normal = plane.normal;

    float distance = (sphere.position - plane.point).dot(plane.normal);
    m.hit = distance < sphere.radius;

    if (m.hit) {
        m.hit = true;
        m.penetration = sphere.radius - distance;
    }
    return m;
}