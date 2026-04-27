#pragma once
#include "Collision.hpp"
#include <array>
#include <vector>

struct PhysicsWorld {
    std::vector<RigidBody> spheres;
    std::array<Plane, 6> planes;
    Vec3 gravity;
    float accumulator;
    float fixedDt;
    float cubeHalfSize;

    float drag;

    PhysicsWorld(float halfSize);

    void update(float realDt);

    void addSphere(Vec3 position, float mass, float radius, float restitution, float friction);
    void clearSpheres();
    const RigidBody& getSphere(int id) const;
    const std::vector<RigidBody>& getSpheres() const;

    void setCubeSize(float halfSize);
    void setDrag(float d);
    void setSpherePosition(int id, const Vec3& pos);
    void setSphereVelocity(int id, const Vec3& vel);
    void setSphereRestitution(int id, float r);
    void setSphereFriction(int id, float f);
    void reset();

    float getCubeSize() const;

    private:
    void step(float dt);
};