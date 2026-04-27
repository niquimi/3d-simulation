#pragma once
#include "RigidBody.hpp"

struct Plane {
    Vec3 point;
    Vec3 normal;
};

struct CollisionManifold {
    bool hit;
    float penetration;
    Vec3 normal;
};

CollisionManifold sphereVsPlane(const RigidBody& sphere, const Plane& plane);
CollisionManifold sphereVsSphere(const RigidBody& a, const RigidBody& b);

void resolveCollision(RigidBody& sphere, const CollisionManifold& manifold);
void resolveSphereSphere(RigidBody& a, RigidBody& b, const CollisionManifold& manifold);