#include "Collision.hpp"
#include <algorithm>

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

CollisionManifold sphereVsSphere(const RigidBody& a, const RigidBody& b) {
    CollisionManifold m{false, 0.f, Vec3 (0,0,0)};
    Vec3 delta = b.position - a.position;
    float dist = delta.length();
    float radiiSum = a.radius + b.radius;
    if (dist < radiiSum) {
        m.hit = true;
        m.normal = delta.normalized();
        m.penetration = radiiSum - dist;
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

    Vec3 tangential = sphere.velocity - sphere.velocity.dot(manifold.normal) * manifold.normal;
    sphere.velocity -= tangential * sphere.friction;
}

void resolveSphereSphere(RigidBody& a, RigidBody& b, const CollisionManifold& manifold) {
    if (!manifold.hit) return;

    float ratio = a.mass / (a.mass + b.mass);
    a.position -= manifold.normal * manifold.penetration * (1 - ratio);
    b.position += manifold.normal * manifold.penetration * ratio;

    Vec3 relVel = b.velocity - a.velocity;
    float vn = relVel.dot(manifold.normal);
    if (vn < 0.0f) {
        float j = -(1.0f + std::min(a.restitution, b.restitution)) * vn / (1.0f / a.mass + 1.0f / b.mass);
        a.velocity -= (j / a.mass) * manifold.normal;
        b.velocity += (j / b.mass) * manifold.normal;
    }
}