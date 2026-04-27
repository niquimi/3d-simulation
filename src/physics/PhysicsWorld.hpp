#pragma once
#include "Collision.hpp"
#include <array>

struct PhysicsWorld {
    RigidBody sphere;
    std::array<Plane, 6> planes;
    Vec3 gravity;
    float accumulator;
    float fixedDt;
    float cubeHalfSize;

    float drag;

    PhysicsWorld(float halfSize);

    void update(float realDt);

    const RigidBody& getSphere() const;

    void setCubeSize(float halfSize);
    void setDrag(float d);
    void setSpherePosition(const Vec3& pos);
    void setSphereVelocity(const Vec3& vel);
    void setSphereRestitution(float r);
    void setSphereFriction(float f);
    void reset();

    const float getCubeSize() const;

    private:
    Vec3 initialPosition;
    Vec3 initialVelocity;

    void step(float dt);
};