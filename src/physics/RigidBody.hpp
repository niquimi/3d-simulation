#pragma once
#include "../math/Vec3.hpp"

struct RigidBody {
    Vec3 position;
    Vec3 velocity;
    Vec3 acceleration;

    float mass;
    float radius;
    float restitution;

private:
    Vec3 accumulatedForce;

public:
    RigidBody(const Vec3& position,
              float mass,
              float radius,
              float restitution);

    void applyForce(const Vec3& force);
    void clearForces();

    Vec3 getAcceleration() const;
};