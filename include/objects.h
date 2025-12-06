#pragma once

#include <SDL.h>
#include <string>
#include "math3d.h"

class World {
public:
    Vec3 camera;
    float yaw;
    float pitch;
    float cameraSpeed;

    Vec3 translate;
    float rotateX;
    float rotateY;

    float rotateSpeedX;
    float rotateSpeedY;

    World();
};

struct Color {
    int r;
    int g;
    int b;
};

struct Edge {
    std::string p1, p2; // nomes dos pontos
    Color color;
};

void drawThickLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float thickness);