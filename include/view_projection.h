#pragma once

#include "math3d.h"
#include "objects.h"
#include "constants.h"
#include <iostream>

Vec3 VRP(World& world);
Vec3 VPN(World& world);
Vec3 VUP(World& world);

Mat4 model_matrix(Vec3 trans, float angleX, float angleY);
Mat4 view_matrix(World& world);
Mat4 projection_matrix();
Mat3 viewport_matrix();

bool clipLine(Vec4& a, Vec4& b);