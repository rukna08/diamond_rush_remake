#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 720


#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include "player.h"
#include "wall.h" 

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
std::vector<Wall> walls;
std::vector<SDL_Rect*> level_grid;
TTF_Font* font;
SDL_Surface* text_surface;
SDL_Texture* text_texture;
int camera_offset = 40;
std::vector<SDL_Texture*> animation_player_idle_list;


bool is_game_running = true;    


// engine_mode = false is game_mode. ;)
bool engine_mode = false;


void draw();
void draw_wall(std::vector<Wall>);
void process_input();
void place_wall(int, int);
void place_wall_pixels(int, int);
void create_level_grid_rects();
void show_grid();
void draw_text(std::string, int, int, SDL_Color*);
void draw_text_init();
void init_animation();
void play_animation(const std::string&);
void update_animation();

SDL_Color color_white = { 255, 255, 255, 255 };

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    player = new Player(renderer, 2, 2);
    


    // Wall Placement.
    for (int i = 0; i < 10; i++) place_wall(i, 0);
    for (int i = 0; i < 10; i++) place_wall(0, i);
    for (int i = 6; i < 15; i++) place_wall(i, 7);
    for (int i = 0; i < 8;  i++) place_wall(9, i);
    for (int i = 0; i < 15; i++) place_wall(i, 9);

    
    // Grid Creation.
    create_level_grid_rects();
        
    
    
    draw_text_init();


    // Initialize Animation System.
    init_animation();

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

int animation_index_player_idle = 0;
int frames = 0;
int animation_speed = 1200;
void draw() {
    play_animation("player_idle");

    if (engine_mode) show_grid();
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    if (engine_mode) draw_text("Engine Mode", 1100, 0, &color_white);
    else draw_text("Game Mode", 1100, 0, &color_white);


    if (engine_mode) {
        // Display Player Position On Screen For Reference.
        std::string position = std::to_string(player->rect.x) + "," + std::to_string(player->rect.y);
        draw_text(position, player->rect.x, player->rect.y, &color_white);
    }

    

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    update_animation();
}

void play_animation(const std::string& animation_name) {
    if (animation_name == "player_idle") {
        if (!engine_mode) {
            player->texture = animation_player_idle_list[animation_index_player_idle];
            SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
        }
        else {
            SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
        }
    }
}

void update_animation() {
    if (frames % animation_speed == 0) {
        animation_index_player_idle++;
    }
    if (animation_index_player_idle == animation_player_idle_list.size() - 1) animation_index_player_idle = 0;
    if (frames == animation_speed) frames = 0;
    frames++;
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
                if (event.key.keysym.sym == SDLK_w && !engine_mode) {
                    player->move("up");
                }
                if (event.key.keysym.sym == SDLK_a && !engine_mode) {
                    player->move("left");
                }
                if (event.key.keysym.sym == SDLK_s && !engine_mode) {
                    player->move("down");
                }
                if (event.key.keysym.sym == SDLK_d && !engine_mode) {
                    player->move("right");
                }
                if (event.key.keysym.sym == SDLK_x) {
                    engine_mode = !engine_mode;
                }

                // Camera Controls.

                if (event.key.keysym.sym == SDLK_w && engine_mode) {
                    for (int i = 0; i < level_grid.size(); i++) {
                        level_grid[i]->y += camera_offset;
                    }
                    for (int i = 0; i < walls.size(); i++) {
                        walls[i].rect.y += camera_offset;
                    }
                    player->rect.y += camera_offset;
                }
                if (event.key.keysym.sym == SDLK_a && engine_mode) {
                    for (int i = 0; i < level_grid.size(); i++) {
                        level_grid[i]->x += camera_offset;
                    }
                    for (int i = 0; i < walls.size(); i++) {
                        walls[i].rect.x += camera_offset;
                    }
                    player->rect.x += camera_offset;
                }
                if (event.key.keysym.sym == SDLK_s && engine_mode) {
                    for (int i = 0; i < level_grid.size(); i++) {
                        level_grid[i]->y -= camera_offset;
                    }
                    for (int i = 0; i < walls.size(); i++) {
                        walls[i].rect.y -= camera_offset;
                    }
                    player->rect.y -= camera_offset;
                }
                if (event.key.keysym.sym == SDLK_d && engine_mode) {
                    for (int i = 0; i < level_grid.size(); i++) {
                        level_grid[i]->x -= camera_offset;
                    }
                    for (int i = 0; i < walls.size(); i++) {
                        walls[i].rect.x -= camera_offset;
                    }
                    player->rect.x -= camera_offset;
                }
            break;
        
            case SDL_QUIT:
                is_game_running = false;
            break;

            case SDL_MOUSEBUTTONDOWN:
                int x = event.motion.x;
                int y = event.motion.y;

                SDL_Point mouse_position = { x, y };

                if (engine_mode) {
                    for (int i = 0; i < level_grid.size(); i++) {
                        if (SDL_PointInRect(&mouse_position, level_grid[i])) {
                            place_wall_pixels(level_grid[i]->x, level_grid[i]->y);
                        }
                    }
                }
            break;

        }
    }
}


// This is in unit level.
void place_wall(int unit_x, int unit_y) {
    walls.emplace_back(SPRITE_SIZE * unit_x, SPRITE_SIZE * unit_y, renderer);
}

// This is in pixel level.
void place_wall_pixels(int x, int y) {
    walls.emplace_back(x, y, renderer);
}

void create_level_grid_rects() {

    for (int y = 0; y < WINDOW_RES_Y*3; y += SPRITE_SIZE) {
        for (int x = 0; x < WINDOW_RES_X*3; x += SPRITE_SIZE) {
            
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

void draw_text_init() {
    font = TTF_OpenFont("data/Roboto-Light.ttf", 250);
}

// Function Generated by ChatGPT, by hate from OpenAI..
void draw_text(std::string text, int x, int y, SDL_Color* color) {
    static std::string last_text = "";
    static SDL_Texture* last_text_texture = nullptr;

    if (text != last_text) {
        if (last_text_texture) {
            SDL_DestroyTexture(last_text_texture); 
        }
        text_surface = TTF_RenderText_Solid(font, text.c_str(), *color);
        last_text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
        SDL_FreeSurface(text_surface);  
        last_text = text;  
    }

    SDL_Rect text_rect = { x, y, 10 * text.length(), 23 };
    SDL_RenderCopy(renderer, last_text_texture, nullptr, &text_rect);
}

void init_animation() {
    
    // Player Idle Animation
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/0.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/1.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/2.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/3.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/4.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/5.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/6.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/7.png"));
    animation_player_idle_list.push_back(IMG_LoadTexture(renderer, "data/animation/player_idle/8.png"));

}

