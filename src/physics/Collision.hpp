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

void resolveCollision(RigidBody& sphere, const CollisionManifold& manifold);