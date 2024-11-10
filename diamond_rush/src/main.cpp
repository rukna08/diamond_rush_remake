#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 1000


#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include "player.h" 
#include "wall.h" 

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
std::vector<Wall> walls;

bool is_game_running = true;


void draw();
void draw_wall(std::vector<Wall>);
void process_input();
void place_wall(int, int);

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    player = new Player(renderer);
    


    // Wall Placement
    for (int i = 0; i < 10; i++) place_wall(i, 0);
    for (int i = 0; i < 10; i++) place_wall(0, i);
    for (int i = 6; i < 15; i++) place_wall(i, 7);
    for (int i = 0; i < 8;  i++) place_wall(9, i);
    for (int i = 0; i < 15; i++) place_wall(i, 9);


    

    //Game Loop
    while (is_game_running) {
        draw();
        draw_wall(walls);
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
    SDL_SetRenderDrawColor(renderer, 0, 22, 51, 255);
    SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);
}

void draw_wall(std::vector<Wall> walls) {
    for (int i = 0; i < walls.size(); i++) {
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
        
            case SDL_QUIT:
                is_game_running = false;
            break;


        }
    }
}
// TODO
void place_wall(int unit_x, int unit_y) {
    walls.emplace_back(SPRITE_SIZE * unit_x, SPRITE_SIZE * unit_y, renderer);
}