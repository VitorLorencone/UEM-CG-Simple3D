#pragma once
#include <cmath>
#include "constants.h"

// Estrutura de vetor com 2 entradas
struct Vec2 {
    float x,y;
    Vec2() : x(0),y(0) {}
    Vec2(float X,float Y):x(X),y(Y){}
};

// Estrutura de vetor com 3 entradas
struct Vec3 {
    float x,y,z;
    Vec3() : x(0),y(0),z(0) {}
    Vec3(float X,float Y,float Z):x(X),y(Y),z(Z){}
    Vec3(Vec4 hp):x(hp.x/hp.w),y(hp.y/hp.w),z(hp.z/hp.w){}
};

// Estrutura de vetor com 4 entradas
struct Vec4 {
    float x,y,z,w;
    Vec4():x(0),y(0),z(0),w(0){}
    Vec4(float X,float Y,float Z,float W):x(X),y(Y),z(Z),w(W){}
    Vec4(Vec3 p):x(p.x),y(p.y),z(p.z),w(1){}
};

// Estrutura de matriz 4x4
struct Mat4 {
    float m[4][4];
    static Mat4 identity(){ Mat4 a; for(int i=0;i<4;i++) for(int j=0;j<4;j++) a.m[i][j]=(i==j)?1.0f:0.0f; return a; }
};

inline Vec3 vec_prod(const Vec3 v, const Vec3 u){
    Vec3 r;
    r.x = v.y*u.z - v.z*u.y;
    r.y = v.z*u.x - v.x*u.z;
    r.z = v.x*u.y - v.y*u.x;
    return r;
}

inline float mod(const Vec3 v){
    return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

// Multiplicação de matriz por vetor
inline Vec4 mul(const Mat4 &M, const Vec4 &v){
    Vec4 r;
    r.x = M.m[0][0]*v.x + M.m[0][1]*v.y + M.m[0][2]*v.z + M.m[0][3]*v.w;
    r.y = M.m[1][0]*v.x + M.m[1][1]*v.y + M.m[1][2]*v.z + M.m[1][3]*v.w;
    r.z = M.m[2][0]*v.x + M.m[2][1]*v.y + M.m[2][2]*v.z + M.m[2][3]*v.w;
    r.w = M.m[3][0]*v.x + M.m[3][1]*v.y + M.m[3][2]*v.z + M.m[3][3]*v.w;
    return r;
}

// Multiplicação de metrizes 4x4
inline Mat4 multiply(const Mat4 &A, const Mat4 &B){
    Mat4 R; for(int i=0;i<4;i++) for(int j=0;j<4;j++){ R.m[i][j]=0; for(int k=0;k<4;k++) R.m[i][j]+=A.m[i][k]*B.m[k][j]; }
    return R;
}

// Matriz de translação
inline Mat4 translate(float tx,float ty,float tz){ 
    Mat4 T = Mat4::identity(); 
    T.m[0][3]=tx; T.m[1][3]=ty; 
    T.m[2][3]=tz; return T; 
}

// Matriz de mudança de escala
inline Mat4 scale(float sx,float sy,float sz){ 
    Mat4 S; 
    for(int i=0;i<4;i++) for(int j=0;j<4;j++) S.m[i][j]=0; 
    S.m[0][0]=sx; 
    S.m[1][1]=sy; 
    S.m[2][2]=sz; 
    S.m[3][3]=1; 
    return S; 
}

// Matriz de rotação em Y
inline Mat4 rotateY(float a){ 
    Mat4 R = Mat4::identity(); 
    float c=cosf(a), s=sinf(a); 
    R.m[0][0]=c; R.m[0][2]=s; 
    R.m[2][0]=-s; R.m[2][2]=c; 
    return R; 
}

// Matriz de rotação em X
inline Mat4 rotateX(float a){ 
    Mat4 R = Mat4::identity(); 
    float c=cosf(a), s=sinf(a); 
    R.m[1][1]=c; R.m[1][2]=-s; 
    R.m[2][1]=s; R.m[2][2]=c; 
    return R; 
}