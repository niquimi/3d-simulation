#pragma once
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <vector>
#include <Windows.h>
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#include "../physics/RigidBody.hpp"
#include "Camera.hpp"
#include <cstdint>


void clear(HDC hdc, RECT rc);

void drawCube(HDC hdc, float halfSize, const Camera& cam, int w, int h);

void drawSpheres(HDC hdc, const std::vector<RigidBody>& spheres, const Camera& cam, int w, int h);

void drawSpheresRaycast(HDC hdc, const std::vector<RigidBody>& spheres, const Camera& cam, int w, int h, Vec3 lightDir, uint8_t* pixelBuffer);