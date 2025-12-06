#include "../include/objects.h"

World::World(){
    this->camera = Vec3(CAMERA_X_START, CAMERA_Y_START, CAMERA_Z_START);
    this->pitch = PITCH_START;
    this->yaw = YAW_START;
    this->cameraSpeed = CAMERA_SPEED;
    this->translate = {0, 0, 0};
    this->rotateX = 0.0f;
    this->rotateY = 0.0f;
    this->rotateSpeedX = 0.0f;
    this->rotateSpeedY = 0.0f;
}

void drawThickLine(SDL_Renderer* renderer, float x1, float y1, float x2, float y2, float thickness) {
    
    if (thickness <= 1.0f) {
        SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
        return;
    }

    float dx = x2 - x1;
    float dy = y2 - y1;
    float length = sqrtf(dx * dx + dy * dy);

    if (length == 0) return;

    float nx = -dy / length;
    float ny = dx / length;

    for (float i = -thickness / 2.0f; i <= thickness / 2.0f; i += 0.5f) {
        float offsetX = nx * i;
        float offsetY = ny * i;
        SDL_RenderDrawLineF(renderer, x1 + offsetX, y1 + offsetY, x2 + offsetX, y2 + offsetY);
    }
}