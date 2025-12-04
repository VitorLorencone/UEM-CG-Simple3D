#include "../include/view_projection.h"

Mat4 projection_matrix(){

    float a = CAMERA_X;
    float b = CAMERA_Y;
    float c = CAMERA_Z;

    // plano de projeção z = 0
    float nx = 0;
    float ny = 0;
    float nz = 1;

    float x0 = 0.0f;
    float y0 = 0.0f;
    float z0 = 0.0f;

    float d0 = x0*nx + y0*ny + z0*nz;
    float d1 = a*nx + b*ny + c*nz;
    float d = d0-d1;

    Mat4 res;
    res.m[0][0] = d+a*nx; res.m[0][1] = a*ny; res.m[0][2] = a*nz; res.m[0][3] = -a*d0;
    res.m[1][0] = b*nx; res.m[1][1] = d+b*ny; res.m[1][2] = b*nz; res.m[1][3] = -b*d0;
    res.m[2][0] = c*nx; res.m[2][1] = c*ny; res.m[2][2] = d+c*nz; res.m[2][3] = -c*d0;
    res.m[3][0] = nx; res.m[3][1] = ny; res.m[3][2] = nz; res.m[3][3] = -d1;

    return res;
}

Point project_point(Mat4 mproj, Point p){
    Vec4 hp = Vec4(p.coord);
    Vec4 projhp = mul(mproj, hp);
    Point projp = Point(Vec3(projhp));
    return projp;
}

Vec3 VRP(){
    return Vec3(CAMERA_X, CAMERA_Y, CAMERA_Z);
}

Vec3 VPN(){
    float x = WORLD_CENTER_X - CAMERA_X;
    float y = WORLD_CENTER_Y - CAMERA_Y;
    float z = WORLD_CENTER_Z - CAMERA_Z;
    float mod = sqrtf(x*x +y*y + z*z);
    return Vec3(x/mod, y/mod, z/mod);
}

Vec3 VUP(){
    return Vec3(0.0f, 1.0f, 0.0f);
}

Mat4 VOM(){
    Vec3 vrp = VRP();
    Vec3 vpn = VPN();
    Vec3 vup = VUP();

    Vec3 n = vpn; // já fez o mod

    Vec3 u = vec_prod(vup, n);
    float modu = mod(u);
    u = Vec3(u.x/modu, u.y/modu, u.z/modu);

    Vec3 v = vec_prod(n, u);

    Mat4 res;
    res.m[0][0] = u.x; res.m[0][1] = u.y; res.m[0][2] = u.z; res.m[0][3] = -(u.x*vrp.x + u.y*vrp.y + u.z*vrp.z);
    res.m[1][0] = v.x; res.m[1][1] = v.y; res.m[1][2] = v.z; res.m[1][3] = -(v.x*vrp.x + v.y*vrp.y + v.z*vrp.z);
    res.m[2][0] = n.x; res.m[2][1] = n.y; res.m[2][2] = n.z; res.m[2][3] = -(n.x*vrp.x + n.y*vrp.y + n.z*vrp.z);
    res.m[3][0] = 0.0f; res.m[3][1] = 0.0f; res.m[3][2] = 0.0f; res.m[3][3] = 1;

    return res;

}

// Fazer matriz NPER