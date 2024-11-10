#define SPRITE_SIZE 64

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include "player.h" 
#include "wall.h" 

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
Wall* wall;

bool is_game_running = true;

void draw();
void process_input();

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(1920, 1080, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    player = new Player(renderer); 
    wall = new Wall(renderer);
    while (is_game_running) {
        draw();
        process_input();
        player->move_constant_right();
        player->move_constant_left();
        player->move_constant_up();
        player->move_constant_down();
    }
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

void draw() {
    SDL_SetRenderDrawColor(renderer, 173, 216, 230, 255);
    SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
    SDL_RenderCopy(renderer, wall->texture, 0, &wall->rect);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                is_game_running = false;
            }
            if (event.key.keysym.sym == SDLK_w) {
                player->move("up");
            }
            if (event.key.keysym.sym == SDLK_a) {
                player->move("left");
            }
            if (event.key.keysym.sym == SDLK_s) {
                player->move("down");
            }
            if (event.key.keysym.sym == SDLK_d) {
                player->move("right");
            }
            break;
        }
    }
}
