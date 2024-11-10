#define SPRITE_SIZE 64
#define WINDOW_RES_X 1080
#define WINDOW_RES_Y 720


#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "player.h" 
#include "wall.h" 

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
Wall* wall;

bool is_game_running = true;
const int wallNumber = 10;


void draw();
void draw_wall(int, std::vector<Wall>);
void process_input();

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_FULLSCREEN, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    player = new Player(renderer); 
    
    // Creating a array of wall pointer objects 
    
    std::vector<Wall> walls;
    //On Horizontal (TOP) Side
    for (int i = 0; i <= wallNumber; i++) {
        walls.emplace_back(SPRITE_SIZE*i,0, renderer);
    }
    //On Vertical (LEFT) Side
    for (int i = 0; i <= wallNumber; i++) {
        walls.emplace_back(0, SPRITE_SIZE * i, renderer);
    }
    //On Horizontal (BOTTOM) Side
    for (int i = 0; i <= wallNumber; i++) {
        walls.emplace_back(SPRITE_SIZE * i, WINDOW_RES_Y, renderer);
    }
    //On Vertical (LEFT) Side
    for (int i = 0; i <= wallNumber; i++) {
        walls.emplace_back(WINDOW_RES_X, SPRITE_SIZE * i, renderer);
    }

    //Game Loop
    while (is_game_running) {
        draw();
        draw_wall(wallNumber,walls);
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
    //SDL_RenderCopy(renderer, wall->texture, 0, &wall->rect);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void draw_wall(int x,std::vector<Wall> walls) {
    for (int i = 0; i <= x*4; i++) {
        SDL_RenderCopy(renderer, walls[i].texture, 0, &walls[i].rect);
    }
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
