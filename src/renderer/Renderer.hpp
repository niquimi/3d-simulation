#pragma once
#include <Windows.h>
#include "../physics/RigidBody.hpp"
#include "Camera.hpp"

void clear(HDC hdc, RECT rc);

void drawCube(HDC hdc, float halfSize, const Camera& cam, int w, int h);

void drawSphere(HDC hdc, const RigidBody& sphere, const Camera& cam, int w, int h);