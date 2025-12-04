#include <SDL.h>
#include <vector>
#include <iostream>
#include <cmath>
#include "math3d.h"
#include "camera.h"

int main(int argc,char**argv){
    if(SDL_Init(SDL_INIT_VIDEO)!=0){ std::cerr<<"SDL_Init error: "<<SDL_GetError()<<"\n"; return 1; }
    const int W=800, H=600;
    SDL_Window* win = SDL_CreateWindow("SDL2 Perspektiva - Cubo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W,H, 0);
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    // Cube vertices (local space)
    std::vector<Vec3> verts = {
        {-1,-1,-1}, {1,-1,-1}, {1,1,-1}, {-1,1,-1}, // back face
        {-1,-1,1},  {1,-1,1},  {1,1,1},  {-1,1,1}   // front face
    };
    // Edges indices
    std::vector<std::pair<int,int>> edges = {{0,1},{1,2},{2,3},{3,0}, {4,5},{5,6},{6,7},{7,4}, {0,4},{1,5},{2,6},{3,7}};

    bool running=true; SDL_Event e;
    float angle=0.0f;

    while(running){
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=false;
            if(e.type==SDL_KEYDOWN && e.key.keysym.sym==SDLK_ESCAPE) running=false;
        }
        angle += 0.01f;

        // Camera & Projection
        Vec3 eye = {0.0f, 0.0f, -6.0f};
        Vec3 center = {0.0f, 0.0f, 0.0f};
        Vec3 up = {0.0f,1.0f,0.0f};
        Mat4 V = lookAt(eye, center, up);
        Mat4 P = perspective(60.0f * 3.14159265f/180.0f, float(W)/float(H), 0.1f, 100.0f);

        // Model transform
        Mat4 M = multiply( translate(0,0,0), multiply( rotateY(angle), rotateX(angle*0.5f) ) );

        SDL_SetRenderDrawColor(ren, 20,20,20,255);
        SDL_RenderClear(ren);

        SDL_SetRenderDrawColor(ren, 255,255,255,255);

        // Project and draw edges
        std::vector<SDL_Point> projected(verts.size());
        for(size_t i=0;i<verts.size();++i){
            Vec4 p = mul(M, Vec4(verts[i].x, verts[i].y, verts[i].z, 1.0f));
            Vec4 vp = mul(V, p);
            Vec4 clip = mul(P, vp);
            if(clip.w==0) clip.w = 1e-6f;
            float nx = clip.x/clip.w; // NDC
            float ny = clip.y/clip.w;
            // map to screen
            int sx = int((nx*0.5f + 0.5f) * W);
            int sy = int((1.0f - (ny*0.5f + 0.5f)) * H);
            projected[i] = {sx, sy};
        }

        for(auto &edge: edges){
            SDL_RenderDrawLine(ren, projected[edge.first].x, projected[edge.first].y, projected[edge.second].x, projected[edge.second].y);
        }

        SDL_RenderPresent(ren);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}