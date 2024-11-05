


#include <SDL.h>
#include <SDL_image.h>

#include <string>

SDL_Window* window;
SDL_Renderer* renderer;

class Player {
public:
    SDL_Rect rect = { 0, 0, 64, 64 };
    SDL_Texture* texture;
    Player() {
        texture = IMG_LoadTexture(renderer, "data/sprite_player.png");
    }
};




Player* player;




void draw();
void process_input();



bool is_game_running = true;



int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    player = new Player();
    while (is_game_running) {
        draw();
        process_input();
    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}



void draw() {
    SDL_FillRect(SDL_GetWindowSurface(window), 0, SDL_MapRGB(SDL_GetWindowSurface(window)->format, 255, 0, 255));
    SDL_UpdateWindowSurface(window);
    SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
    SDL_RenderPresent(renderer);
}



void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_game_running = false;
                }
            break;
        }
    }
}