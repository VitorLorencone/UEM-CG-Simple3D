#pragma once

// Math
#define PI 3.14159265f
#define TWO_PI 2*PI
#define CLIP_MIN 1e-6f

// Render
#define FRAMERATE 60
#define REFRESH_TIME_MS 1000/FRAMERATE

// Colors
#define BACKGROUND_R 30
#define BACKGROUND_G 30
#define BACKGROUND_B 30
#define BACKGROUND_A 255
#define OBJ_R 255
#define OBJ_G 0
#define OBJ_B 0
#define OBJ_A 255
#define X_AXIS_R 255
#define X_AXIS_G 0
#define X_AXIS_B 0
#define Y_AXIS_R 0
#define Y_AXIS_G 255
#define Y_AXIS_B 0
#define Z_AXIS_R 0
#define Z_AXIS_G 0
#define Z_AXIS_B 255

// Camera (VRP)
#define CAMERA_X_START 1.0f
#define CAMERA_Y_START 1.0f
#define CAMERA_Z_START 5.0f
#define CAMERA_SPEED 0.1f
#define YAW_START 0.0f
#define PITCH_START 0.0f
#define SENSITIVITY 0.002f
#define FOV 60.0f * PI / 180.0f
#define NEAR 0.01f
#define FAR 100.0f
#define PITCH_MAX 1.5f
#define PITCH_MIN -1.5f

// World -> LookAt
#define WINDOW_SCREEN_WIDTH 1280
#define WINDOW_SCREEN_HEIGHT 720
#define WORLD_CENTER_X 0.0f
#define WORLD_CENTER_Y 0.0f
#define WORLD_CENTER_Z 0.0f
#define MAX_AXIS 1000
#define MIN_AXIS -1000

// Viewport
#define VIEWPORT_SCREEN_WIDTH 1280
#define VIEWPORT_SCREEN_HEIGHT 720

// Thickness
#define THICKNESS_FACTOR 20.0f
#define MAX_THICKNESS 5.0f
#define MIN_THICKNESS 1.0f