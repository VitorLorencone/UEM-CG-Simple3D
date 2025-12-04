#pragma once

#include <SDL.h>
#include "math3d.h"

// Classe Object
class Object {
public:
    //virtual int draw(SDL_Renderer *ren) = 0;
};

// Classe Point
class Point : public Object{    
public:
    Vec3 coord;

    Point();
    Point(float x, float y, float z);
    Point(Vec3 p);
    //int draw(SDL_Renderer *ren) override;
};

// Classe Segment
class Segment : public Object{   
public:
    Point p;
    Point q;

    Segment(float x1, float y1, float z1, float x2, float y2, float z2);
    Segment(Point p1, Point p2);
    //int draw(SDL_Renderer *ren) override;
};