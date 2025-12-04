#include "../include/objects.h"

// Definições
Point::Point(){
    this->coord = Vec3(0, 0, 0);
}

Point::Point(float x, float y, float z){
    this->coord = Vec3(x, y, z);
}

Point::Point(Vec3 p){
    this->coord = p;
}

Segment::Segment(float x1, float y1, float z1, float x2, float y2, float z2){
    this->p.coord = Vec3(x1, y1, z1);
    this->q.coord = Vec3(x2, y2, z2);
}

Segment::Segment(Point p1, Point p2){
    this->p = p1;
    this->q = p2;
}