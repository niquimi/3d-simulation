#pragma once
#include <vector>
#include <Windows.h>
#include "../physics/RigidBody.hpp"
#include "Camera.hpp"

void clear(HDC hdc, RECT rc);

void drawCube(HDC hdc, float halfSize, const Camera& cam, int w, int h);

void drawSpheres(HDC hdc, const std::vector<RigidBody>& spheres, const Camera& cam, int w, int h);