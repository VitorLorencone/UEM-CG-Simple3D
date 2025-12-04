#include <SDL.h>
#include <iostream>
#include "../include/constants.h"
#include "../include/math3d.h"

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
    SDL_Event event;

    // Loop principal
    while (running) {

        // Usuário clica no "X"
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {running = false;}
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoint(renderer, 100, 100);
        SDL_RenderDrawPoint(renderer, 150, 150);
        SDL_RenderDrawLine(renderer, 100, 100, 150, 150);

        // Renderiza o conteúdo atual no render em FRAMERATE frames por segundo
        SDL_RenderPresent(renderer);
        SDL_Delay(REFRESH_TIME_MS);
    }

    // Finaliza a janela
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
