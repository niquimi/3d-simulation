#include "PhysicsWorld.hpp"

PhysicsWorld::PhysicsWorld(float h)
    : sphere(Vec3(0, 0, 0), 1.0f, 0.5f, 0.8f),
      gravity(0.0f, -9.81f, 0.0f),
      accumulator(0.0f),
      fixedDt(1.0f / 120.0f)
{
    // Planos del cubo
    planes[0] = { Vec3(0, -h, 0), Vec3(0, 1, 0) };   // suelo
    planes[1] = { Vec3(0,  h, 0), Vec3(0,-1, 0) };   // techo
    planes[2] = { Vec3(-h, 0, 0), Vec3(1, 0, 0) };   // izquierda
    planes[3] = { Vec3( h, 0, 0), Vec3(-1,0, 0) };   // derecha
    planes[4] = { Vec3(0, 0,  h), Vec3(0, 0,-1) };   // frente
    planes[5] = { Vec3(0, 0, -h), Vec3(0, 0, 1) };   // fondo
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

    Vec3 acceleration = sphere.getAcceleration();
    sphere.velocity += acceleration * dt;
    sphere.position += sphere.velocity * dt;

    for (int i = 0; i < 2; i++) {
        for (const Plane& plane : planes) {
            CollisionManifold m = sphereVsPlane(sphere, plane);
            if (m.hit) {
                // Si hay colisión, resolverla
                resolveCollision(sphere, m);
            }
        }
    }
}

const RigidBody& PhysicsWorld::getSphere() const {
    return sphere;
}