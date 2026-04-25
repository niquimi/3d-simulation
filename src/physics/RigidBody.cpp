#include "RigidBody.hpp"

RigidBody::RigidBody(const Vec3& position,
                 float mass,
                 float radius,
                 float restitution)
    : position(position),
      velocity(Vec3::zero()),
      acceleration(Vec3::zero()),
      mass(mass),
      radius(radius),
      restitution(restitution),
      accumulatedForce(Vec3::zero())
{
}

void RigidBody::applyForce(const Vec3& force)
{
    accumulatedForce += force;
}

void RigidBody::clearForces() {
    accumulatedForce = Vec3::zero();
}

Vec3 RigidBody::getAcceleration() const {
    if (mass < 1e-8f) return Vec3::zero();
    return accumulatedForce / mass;
}