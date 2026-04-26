#pragma once
#include "Collision.hpp"
#include <array>

struct PhysicsWorld {
    RigidBody sphere;
    std::array<Plane, 6> planes;
    Vec3 gravity;
    float accumulator;
    float fixedDt;

    PhysicsWorld(float halfSize);

    void update(float realDt);

    const RigidBody& getSphere() const;

    private:
    void step(float dt);
};