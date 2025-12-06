#include <SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <string>
#include "../include/constants.h"
#include "../include/math3d.h"
#include "../include/objects.h"
#include "../include/view_projection.h"

int main(int argc, char** argv) {

    // Erro de inicialização da janela
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erro ao inicializar SDL: " << SDL_GetError() << "\n";
        return 1;
    }

    // Cria window
    SDL_Window* window = SDL_CreateWindow(
        "SIMPLE3D", 
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SCREEN_WIDTH, WINDOW_SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    // Erro de criação da janela
    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    // Cria renderer e erro de criação abaixo
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Erro ao criar renderer: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Estrutura de eventos
    bool running = true;
    SDL_SetRelativeMouseMode(SDL_TRUE);
    SDL_Event event;

    // Definições Globais
    World world = World();

    // Corrigir
    std::string filename = "scene.txt";

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo scene.txt, garanta que ele exista no mesmo diretório\n";
        return 1;
    }

    std::unordered_map<std::string, Vec3> verts;
    std::vector<Edge> edges;
    Color currentRGB = {OBJ_R, OBJ_G, OBJ_B};

    // Leitura do arquivo scene.txt
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "POINT") {
            std::string name;
            float x, y, z;
            ss >> name >> x >> y >> z;
            verts[name] = {x, y, z};

        }else if (type == "SEGMENT") {
            std::string p1, p2;
            ss >> p1 >> p2;
            edges.push_back({p1, p2, currentRGB});

        }else if (type == "RGB") {
            int r, g, b;
            ss >> r >> g >> b;
            currentRGB = {r, g, b};

        }else if (type == "TRIANGLE"){
            std::string p1, p2, p3;
            ss >> p1 >> p2 >> p3;
            edges.push_back({p1, p2, currentRGB});
            edges.push_back({p2, p3, currentRGB});
            edges.push_back({p3, p1, currentRGB});

        }else if (type == "TRANSLATE"){
            float x, y, z;
            ss >> x >> y >> z;
            world.translate = {x, y, z};
        
        }else if (type == "ROTATE_X"){
            float x;
            ss >> x;
            world.rotateX = x * PI / 180.0f;

        }else if (type == "ROTATE_Y"){
            float y;
            ss >> y;
            world.rotateY = y * PI / 180.0f;
        
        }else if (type == "AUTO_ROTATE_X"){
            float x;
            ss >> x;
            world.rotateSpeedX = x;

        }else if (type == "AUTO_ROTATE_Y"){
            float y;
            ss >> y;
            world.rotateSpeedY = y;
        
        }else {
            std::cerr << "Linha ignorada: " << line << "\n";
        }
    }

    file.close();

    // Definição dos eixos coordenados
    std::unordered_map<std::string, Vec3> axis_verts;
    std::vector<Edge> axis_edges;

    axis_verts["__x1__"] = {MAX_AXIS, 0, 0}; axis_verts["__x2__"] = {MIN_AXIS, 0, 0};
    axis_verts["__y1__"] = {0, MAX_AXIS, 0}; axis_verts["__y2__"] = {0, MIN_AXIS, 0};
    axis_verts["__z1__"] = {0, 0, MAX_AXIS}; axis_verts["__z2__"] = {0, 0, MIN_AXIS};
    axis_edges.push_back({"__x1__", "__x2__", {X_AXIS_R, X_AXIS_G, X_AXIS_B}});
    axis_edges.push_back({"__y1__", "__y2__", {Y_AXIS_R, Y_AXIS_G, Y_AXIS_B}});
    axis_edges.push_back({"__z1__", "__z2__", {Z_AXIS_R, Z_AXIS_G, Z_AXIS_B}});

    // Criação das Matrizes Fixas;
    Mat4 P = projection_matrix();
    Mat3 VP = viewport_matrix();

    // Loop principal
    while (running) {
        Uint32 frameStart = SDL_GetTicks();

        // Usuário clica no "X" ou "Esc"
        while (SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT) {running = false;}
            if(event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {running = false;}
        }

        // Movimentação WASD, SPACE e LCTRL
        const Uint8* keystate = SDL_GetKeyboardState(NULL);
        Vec3 forward = VPN(world);
        Vec3 right = vec_prod(forward, Vec3(0,1,0));
        float magRight = sqrtf(right.x*right.x + right.y*right.y + right.z*right.z);
        if(magRight != 0) right = Vec3(right.x/magRight, right.y/magRight, right.z/magRight);

        if(keystate[SDL_SCANCODE_W]) world.camera = world.camera + forward * world.cameraSpeed;
        if(keystate[SDL_SCANCODE_S]) world.camera = world.camera - forward * world.cameraSpeed;
        if(keystate[SDL_SCANCODE_D]) world.camera = world.camera - right * world.cameraSpeed;
        if(keystate[SDL_SCANCODE_A]) world.camera = world.camera + right * world.cameraSpeed;

        // subir/descendo
        if(keystate[SDL_SCANCODE_SPACE]) world.camera.y += world.cameraSpeed;
        if(keystate[SDL_SCANCODE_LCTRL]) world.camera.y -= world.cameraSpeed;

        // Controle Mouse
        int mouseX, mouseY;
        Uint32 buttons = SDL_GetRelativeMouseState(&mouseX, &mouseY);

        // Botão esquerdo pressionado
        if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            world.yaw   -= mouseX * SENSITIVITY;
            world.pitch += -mouseY * SENSITIVITY;

            // limita pitch para não virar de cabeça para baixo
            if(world.pitch > PITCH_MAX) world.pitch =  PITCH_MAX;
            if(world.pitch < PITCH_MIN) world.pitch = PITCH_MIN;
        }

        // Inicializa matrizes de transformações variáveis
        Mat4 M = model_matrix({0,0,0}, 0.0f, 0.0f);
        Mat4 V = view_matrix(world);

        // Cor de Background
        SDL_SetRenderDrawColor(renderer, BACKGROUND_R, BACKGROUND_G, BACKGROUND_B, BACKGROUND_A);
        SDL_RenderClear(renderer);

        for(int i = 0; i < 3; i++){
            Vec4 cp1 = axis_verts[axis_edges[i].p1]; Vec4 cp2 = axis_verts[axis_edges[i].p2];

            Vec4 p1 = mul(M, cp1); Vec4 p2 = mul(M, cp2);
            Vec4 vp1 = mul(V, p1); Vec4 vp2 = mul(V, p2);
            Vec4 clip1 = mul(P, vp1); Vec4 clip2 = mul(P, vp2);

            if(!clipLine(clip1, clip2)) continue;

            if (clip1.w < 0.001f) clip1.w = 0.001f; 
            if (clip2.w < 0.001f) clip2.w = 0.001f;

            Vec3 ndc1 = Vec3(clip1.x/clip1.w, clip1.y/clip1.w, clip1.z/clip1.w); Vec3 ndc2 = Vec3(clip2.x/clip2.w, clip2.y/clip2.w, clip2.z/clip2.w);
            Vec3 fp1 = mul3(VP, ndc1); Vec3 fp2 = mul3(VP, ndc2);

            SDL_SetRenderDrawColor(renderer, axis_edges[i].color.r, axis_edges[i].color.g, axis_edges[i].color.b, OBJ_A);
            SDL_RenderDrawLineF(renderer, fp1.x, fp1.y, fp2.x, fp2.y);
        }

        // Model matrix e auto rotate
        M = model_matrix(world.translate, world.rotateX, world.rotateY);
        world.rotateX += world.rotateSpeedX;
        world.rotateY += world.rotateSpeedY;

        // Cálculo para objetos em tela
        for(auto &edge: edges){
            Vec4 cp1 = verts[edge.p1]; Vec4 cp2 = verts[edge.p2];

            Vec4 p1 = mul(M, cp1); Vec4 p2 = mul(M, cp2);
            Vec4 vp1 = mul(V, p1); Vec4 vp2 = mul(V, p2);
            Vec4 clip1 = mul(P, vp1); Vec4 clip2 = mul(P, vp2);

            if(!clipLine(clip1, clip2)) continue;

            if (clip1.w < 0.001f) clip1.w = 0.001f;
            if (clip2.w < 0.001f) clip2.w = 0.001f;

            Vec3 ndc1 = Vec3(clip1.x/clip1.w, clip1.y/clip1.w, clip1.z/clip1.w); Vec3 ndc2 = Vec3(clip2.x/clip2.w, clip2.y/clip2.w, clip2.z/clip2.w);
            Vec3 fp1 = mul3(VP, ndc1); Vec3 fp2 = mul3(VP, ndc2);

            // Profundidade para thickness
            float avgDepth = (clip1.w + clip2.w)/2.0f;
            float thickness = THICKNESS_FACTOR/avgDepth;

            if (thickness > MAX_THICKNESS) thickness = MAX_THICKNESS;
            if (thickness < MIN_THICKNESS) thickness = MIN_THICKNESS;

            SDL_SetRenderDrawColor(renderer, edge.color.r, edge.color.g, edge.color.b, OBJ_A);
            drawThickLine(renderer, fp1.x, fp1.y, fp2.x, fp2.y, thickness);
        }
        SDL_RenderPresent(renderer);

        // Renderiza o conteúdo atual no render em FRAMERATE frames por segundo
        Uint32 frameEnd = SDL_GetTicks();
        Uint32 frameDuration = frameEnd - frameStart;
        if (frameDuration < REFRESH_TIME_MS) {
            // Atraso apenas se o frame for renderizado mais rápido que o desejado
            SDL_Delay(REFRESH_TIME_MS - frameDuration);
        }
    }

    // Finaliza a janela
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}