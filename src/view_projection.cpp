#include "../include/view_projection.h"

Vec3 VRP(World& world){
    return world.camera;
}

// Função VPN com câmera FIXA
/* Vec3 VPN(World& world){
    float x = WORLD_CENTER_X - world.camera.x;
    float y = WORLD_CENTER_Y - world.camera.y;
    float z = WORLD_CENTER_Z - world.camera.z;
    float mod = sqrtf(x*x + y*y + z*z);
    return Vec3(x/mod, y/mod, z/mod);
} */

Vec3 VPN(World& world){
    float cosPitch = cosf(world.pitch);
    Vec3 dir;
    dir.x = cosPitch * sinf(world.yaw);
    dir.y = sinf(world.pitch);
    dir.z = cosPitch * cosf(world.yaw);

    // normaliza
    float mod = sqrtf(dir.x*dir.x + dir.y*dir.y + dir.z*dir.z);

    return Vec3(dir.x/mod, dir.y/mod, -dir.z/mod);
}

Vec3 VUP(World& world){
    return Vec3(0.0f, 1.0f, 0.0f);
}

Mat4 model_matrix(Vec3 trans, float angleX, float angleY){
    return multiply(translate(trans.x, trans.y, trans.z), multiply(rotateY(angleY), rotateX(angleX)));
}

Mat4 view_matrix(World& world){
    Vec3 vrp = VRP(world);
    Vec3 vpn = VPN(world);
    Vec3 vup = VUP(world);

    Vec3 n = vpn; // já fez o mod

    Vec3 u = vec_prod(vup, n);
    float modu = mod(u);
    u = Vec3(u.x/modu, u.y/modu, u.z/modu);

    Vec3 v = vec_prod(n, u);

    Mat4 res;
    res.m[0][0] = u.x; res.m[0][1] = u.y; res.m[0][2] = u.z; res.m[0][3] = -(u.x*vrp.x + u.y*vrp.y + u.z*vrp.z);
    res.m[1][0] = v.x; res.m[1][1] = v.y; res.m[1][2] = v.z; res.m[1][3] = -(v.x*vrp.x + v.y*vrp.y + v.z*vrp.z);
    res.m[2][0] = -n.x; res.m[2][1] = -n.y; res.m[2][2] = -n.z; res.m[2][3] = (n.x*vrp.x + n.y*vrp.y + n.z*vrp.z);
    res.m[3][0] = 0.0f; res.m[3][1] = 0.0f; res.m[3][2] = 0.0f; res.m[3][3] = 1;

    return res;

}

// Função de projeção com definição do plano de projeção e sua normal, troquei para a que usa FOV
/* Mat4 projection_matrix(World& world){

    float aspect = (float) WINDOW_SCREEN_WIDTH/WINDOW_SCREEN_HEIGHT;

    float a = world.camera.x;
    float b = world.camera.y;
    float c = world.camera.z;

    // plano de projeção z = 0
    float nx = 0;
    float ny = 0;
    float nz = 1;

    float x0 = WORLD_CENTER_X;
    float y0 = WORLD_CENTER_Y;
    float z0 = WORLD_CENTER_Z;

    float d0 = x0*nx + y0*ny + z0*nz;
    float d1 = a*nx + b*ny + c*nz;
    float d = d0-d1;

    Mat4 res;
    res.m[0][0] = d+a*nx; res.m[0][1] = a*ny; res.m[0][2] = a*nz; res.m[0][3] = -a*d0;
    res.m[1][0] = b*nx; res.m[1][1] = d+b*ny; res.m[1][2] = b*nz; res.m[1][3] = -b*d0;
    res.m[2][0] = c*nx; res.m[2][1] = c*ny; res.m[2][2] = d+c*nz; res.m[2][3] = -c*d0;
    res.m[3][0] = nx; res.m[3][1] = ny; res.m[3][2] = nz; res.m[3][3] = -d1;
    res.m[0][0] /= aspect;

    return res;
} */

Mat4 projection_matrix(){
    float aspect = (float)WINDOW_SCREEN_WIDTH / WINDOW_SCREEN_HEIGHT;
    
    float fov = FOV;
    float near = NEAR;
    float far  = FAR;

    float f = 1.0f / tanf(fov / 2.0f);

    Mat4 res = Mat4(); // inicializa com zeros
    res.m[0][0] = f / aspect;
    res.m[1][1] = f;
    res.m[2][2] = (far + near) / (near - far);
    res.m[2][3] = (2.0f * far * near) / (near - far);
    res.m[3][2] = -1.0f;
    res.m[3][3] = 0.0f;

    return res;
}

Mat3 viewport_matrix(){
    float rw = (float) WINDOW_SCREEN_WIDTH/WINDOW_SCREEN_HEIGHT;
    float rv = (float) VIEWPORT_SCREEN_WIDTH/VIEWPORT_SCREEN_HEIGHT;

    float xmin = -1;
    float xmax = 1;
    float ymin = -1;
    float ymax = 1;

    float umin = 0;
    float umax = (float) VIEWPORT_SCREEN_WIDTH;
    float vmin = 0;
    float vmax = (float) VIEWPORT_SCREEN_HEIGHT;

    float sx = (umax-umin)/(xmax-xmin);
    float sy = (vmax-vmin)/(ymax-ymin);

    Mat3 res;
    if(rw > rv){
        float vmaxnovo = (umax-umin)/rw + vmin;
        res.m[0][0] = sx; res.m[0][1] = 0; res.m[0][2] = umin - sx*xmin;
        res.m[1][0] = 0; res.m[1][1] = -sy; res.m[1][2] = sy*ymax + vmax/2 - vmaxnovo/2 + vmin;
        res.m[2][0] = 0; res.m[2][1] = 0; res.m[2][2] = 1;
    }else{
        float umaxnovo = rw*(vmax-vmin) + umin;
        res.m[0][0] = sx; res.m[0][1] = 0; res.m[0][2] = -sx*xmin + umax/2 - umaxnovo/2 + umin;
        res.m[1][0] = 0; res.m[1][1] = -sy; res.m[1][2] = sy*ymax + vmin;
        res.m[2][0] = 0; res.m[2][1] = 0; res.m[2][2] = 1;
    }

    return res;
}

bool clipLine(Vec4& a, Vec4& b) {
    float d_a = a.w + a.z;
    float d_b = b.w + b.z;

    // Ambos atrás do Near Plane (invisíveis)
    if (d_a < 0 && d_b < 0) return false;

    // Se um está fora e o outro dentro, interpolar
    if (d_a < 0) { // A está fora
        float t = d_a / (d_a - d_b);
        a = a + (b - a) * t;
    } else if (d_b < 0) { // B está fora
        float t = d_b / (d_b - d_a);
        b = b + (a - b) * t;
    }

    d_a = a.w - a.z;
    d_b = b.w - b.z;

    // Ambos além do Far Plane (invisíveis)
    if (d_a < 0 && d_b < 0) return false;

    if (d_a < 0) { // A está fora (muito longe)
        float t = d_a / (d_a - d_b);
        a = a + (b - a) * t;
    } else if (d_b < 0) { // B está fora (muito longe)
        float t = d_b / (d_b - d_a);
        b = b + (a - b) * t;
    }

    return true;
};