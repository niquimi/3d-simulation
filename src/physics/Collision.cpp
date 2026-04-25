#include "Collision.hpp"

CollisionManifold sphereVsPlane(const RigidBody& sphere, const Plane& plane) {
    CollisionManifold m{false, 0.f, plane.normal.normalized()};

    float distance = (sphere.position - plane.point).dot(plane.normal);

    const float slop = 1e-5f;
    if (distance < sphere.radius - slop) {
        m.hit = true;
        m.penetration = sphere.radius - distance;
    }
    return m;
}

void resolveCollision(RigidBody& sphere, const CollisionManifold& manifold) {
    if (!manifold.hit) return;

    // Correct position
    sphere.position += manifold.normal * manifold.penetration;

    float vn = sphere.velocity.dot(manifold.normal);
    if (vn < 0.0f) {
        // Reflect velocity
        sphere.velocity -= (1.0f + sphere.restitution) * vn * manifold.normal;
    }
}