#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld(float h)
    : sphere(Vec3(0, 0, 0), 1.0f, 1.0f, 0.8f, 0.05f),
      gravity(0.0f, -9.81f, 0.0f),
      accumulator(0.0f),
      fixedDt(1.0f / 120.0f),
      drag(0.05f),
      initialPosition(Vec3(0, 0, 0)),
      initialVelocity(Vec3(0, 0, 0)),
      cubeHalfSize(h)
{
    setCubeSize(h);
}

void PhysicsWorld::update(float realDt) {
    accumulator += realDt;
    while (accumulator >= fixedDt) {
        step(fixedDt);
        accumulator -= fixedDt;
    }
}

void PhysicsWorld::step(float dt) {

    sphere.clearForces();

    sphere.applyForce(gravity * sphere.mass);
    sphere.applyForce(sphere.velocity * -drag);

    Vec3 acceleration = sphere.getAcceleration();
    sphere.velocity += acceleration * dt;
    sphere.position += sphere.velocity * dt;

    for (int i = 0; i < planes.size()/2; i++) {
        for (const Plane& plane : planes) {
            CollisionManifold m = sphereVsPlane(sphere, plane);
            if (m.hit) {
                // Si hay colisión, resolverla
                resolveCollision(sphere, m);
            }
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

void PhysicsWorld::setSpherePosition(const Vec3& pos) {
    sphere.position = pos;
}

void PhysicsWorld::setSphereVelocity(const Vec3& vel) {
    sphere.velocity = vel;
}

void PhysicsWorld::setSphereRestitution(float r) {
    sphere.restitution = r;
}

void PhysicsWorld::setSphereFriction(float f) {
    sphere.friction = f;
}

void PhysicsWorld::reset() {
    sphere.position = initialPosition;
    sphere.velocity = initialVelocity;
    accumulator = 0.0f;
}

const RigidBody& PhysicsWorld::getSphere() const {
    return sphere;
}

const float PhysicsWorld::getCubeSize() const {
    return cubeHalfSize;
}