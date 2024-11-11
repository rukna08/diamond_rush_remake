#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
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
std::vector<Wall> walls;
std::vector<SDL_Rect*> level_grid;

bool is_game_running = true;


void draw();
void draw_wall(std::vector<Wall>);
void process_input();
void place_wall(int, int);
void create_level_grid_rects();
void show_grid();

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    player = new Player(renderer, 2, 2);
    


    // Wall Placement.
    for (int i = 0; i < 10; i++) place_wall(i, 0);
    for (int i = 0; i < 10; i++) place_wall(0, i);
    for (int i = 6; i < 15; i++) place_wall(i, 7);
    for (int i = 0; i < 8;  i++) place_wall(9, i);
    for (int i = 0; i < 15; i++) place_wall(i, 9);

    // Grid Creation.
    create_level_grid_rects();
    

    // Game Loop.
    while (is_game_running) {
        draw();
        draw_wall(walls);
        process_input();
        player->check_collision(walls);
        player->move_constant_right();
        player->move_constant_left();
        player->move_constant_up();
        player->move_constant_down();
    }




    // Unload
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}

void draw() {
    SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
    show_grid();
    SDL_SetRenderDrawColor(renderer, 0, 22, 51, 255);
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

void create_level_grid_rects() {
    
    //
    //
    // Creating 512x512 grid for testing.
    // 512 because it is a multiple of SPRITE_SIZE i.e. 64
    //
    //

    for (int y = 0; y <= 512; y += SPRITE_SIZE) {
        for (int x = 0; x <= 512; x += SPRITE_SIZE) {
            
            level_grid.push_back(new SDL_Rect{ x, y, SPRITE_SIZE, SPRITE_SIZE } );

        }
    }
}

void show_grid() {
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    
    for (int i = 0; i < level_grid.size(); i++) {
        SDL_RenderDrawRect(renderer, level_grid[i]);
    }

}