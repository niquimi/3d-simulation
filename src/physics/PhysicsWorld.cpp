#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld(float h)
    : gravity(0.0f, -9.81f, 0.0f),
      accumulator(0.0f),
      fixedDt(1.0f / 120.0f),
      drag(0.05f),
      cubeHalfSize(h)
{
    setCubeSize(h);
}

void PhysicsWorld::addSphere(Vec3 position, float mass, float radius, float restitution, float friction) {
    spheres.emplace_back(position, mass, radius, restitution, friction);
}

void PhysicsWorld::clearSpheres() {
    spheres.clear();
}

const std::vector<RigidBody>& PhysicsWorld::getSpheres() const {
    return spheres;
}

void PhysicsWorld::update(float realDt) {
    accumulator += realDt;
    while (accumulator >= fixedDt) {
        step(fixedDt);
        accumulator -= fixedDt;
    }
}

void PhysicsWorld::step(float dt) {

    // Forces + integration
    for (RigidBody& s : spheres) {
        s.clearForces();
        s.applyForce(gravity * s.mass);
        s.applyForce(s.velocity * -drag);

        Vec3 acceleration = s.getAcceleration();
        s.velocity += acceleration * dt;
        s.position += s.velocity * dt;
    }

    // Sphere-plane collision
    for (RigidBody& s : spheres) {
        for (int iter = 0; iter < 2; iter++) {
            for (const Plane& plane : planes) {
                CollisionManifold m = sphereVsPlane(s, plane);
                if (m.hit) resolveCollision(s, m);
            }
        }
    }

    // Sphere-sphere collision
    for (int i = 0; i < (int)spheres.size(); i++) {
        for (int j = i + 1; j < (int)spheres.size(); j++) {
            CollisionManifold m = sphereVsSphere(spheres[i], spheres[j]);
            if (m.hit) resolveSphereSphere(spheres[i], spheres[j], m);
        }
    }
}

void PhysicsWorld::setCubeSize(float h) {
    cubeHalfSize = h;
    planes[0] = { Vec3(0, -h, 0), Vec3(0,  1, 0) };
    planes[1] = { Vec3(0,  h, 0), Vec3(0, -1, 0) };
    planes[2] = { Vec3(-h, 0, 0), Vec3( 1, 0, 0) };
    planes[3] = { Vec3( h, 0, 0), Vec3(-1, 0, 0) };
    planes[4] = { Vec3(0, 0,  h), Vec3(0, 0, -1) };
    planes[5] = { Vec3(0, 0, -h), Vec3(0, 0,  1) };
}

void PhysicsWorld::setDrag(float d) {
    drag = d;
}

void PhysicsWorld::setSpherePosition(int id, const Vec3& pos) {
    if (id < 0 || id >= (int)spheres.size()) return;
    spheres[id].position = pos;
}

void PhysicsWorld::setSphereVelocity(int id, const Vec3& vel) {
    if (id < 0 || id >= (int)spheres.size()) return;
    spheres[id].velocity = vel;
}

void PhysicsWorld::setSphereRestitution(int id, float r) {
    if (id < 0 || id >= (int)spheres.size()) return;
    spheres[id].restitution = r;
}

void PhysicsWorld::setSphereFriction(int id, float f) {
    if (id < 0 || id >= (int)spheres.size()) return;
    spheres[id].friction = f;
}

void PhysicsWorld::reset() {
    spheres.clear();
    accumulator = 0.0f;
}

const RigidBody& PhysicsWorld::getSphere(int id) const {
    return spheres[id];
}

float PhysicsWorld::getCubeSize() const {
    return cubeHalfSize;
}