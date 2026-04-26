#include <iostream>
#include "physics/PhysicsWorld.hpp"

int main() {
    PhysicsWorld world(10.0f);
    for (int i = 0; i < 300; i++) {
        world.update(1.0f / 60.0f);
        Vec3 p = world.getSphere().position;
        std::cout << "Sphere Position Y: " << p.y << std::endl;
    }
    return 0;
}