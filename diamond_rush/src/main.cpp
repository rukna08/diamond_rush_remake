#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 720
#define ANGKOR_WAT "data/map_config_angkor_wat.level"



#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "player.h"
#include "wall.h" 
#include "map_manager.h"
#include "grid_manager.h"
#include <stack>
#include <iostream>


SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
std::vector<Wall> walls;
std::vector<SDL_Rect*> level_grid;
std::stack<char> direction_stream;

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

void draw_text(std::string, int, int, SDL_Color*);
void draw_text_init();
void init_animation();
void play_animation(const std::string&);
void update_animation();
void remove_wall_pixels(int, int);
void map_reset();


SDL_Color color_white = { 255, 255, 255, 255 };

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    player = new Player(renderer, 2, 2);


    // Load the game map.
    reload_map(walls,renderer);


    // Grid Creation.
    create_level_grid_rects(level_grid);



    draw_text_init();


    // Initialize Animation System.
    init_animation();

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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


std::string current_idle_animation = "player_idle_right";
int animation_index_player_idle = 0;
int frames = 0;
int animation_speed = 1200;
void draw() {
    play_animation(current_idle_animation);

    if (engine_mode) {
        show_grid(level_grid, renderer);
    }

    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    if (engine_mode) draw_text("Engine Mode", 1100, 0, &color_white);
    else draw_text("Game Mode", 1100, 0, &color_white);

    if (engine_mode) {
        // Display Player Position On Screen For Reference.
        std::string position = std::to_string(player->rect.x) + "," + std::to_string(player->rect.y);
        draw_text(position, player->rect.x, player->rect.y, &color_white);
        draw_text("Save Level    - R", 1100, 20, &color_white);
        draw_text("Destroy Level - L", 1100, 40, &color_white);
        draw_text("Reload Level  - Y", 1100, 60, &color_white);
    }



    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    update_animation();
}

void play_animation(const std::string& animation_name) {
    if (animation_name == "player_idle_right") {
        if (!engine_mode) {
            player->texture = animation_player_idle_list[animation_index_player_idle];
            std::cout << "animation_index_player_idle: " << animation_index_player_idle << std::endl;
            SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
        }
        else {
            SDL_RenderCopy(renderer, player->texture, 0, &player->rect);
        }
    }

    if (animation_name == "player_idle_left") {
        if (!engine_mode) {
            player->texture = animation_player_idle_list[animation_index_player_idle];
            SDL_RenderCopyEx(renderer, player->texture, 0, &player->rect, 0, 0, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopyEx(renderer, player->texture, 0, &player->rect, 0, 0, SDL_FLIP_HORIZONTAL);
        }
    }
}

void update_animation() {
    if (frames % animation_speed == 0) {
        animation_index_player_idle++;
    }
    if (animation_index_player_idle == animation_player_idle_list.size()) animation_index_player_idle = 0;
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
                map_reset();
                save_map(walls, 0);
                is_game_running = false;
            }
            if (event.key.keysym.sym == SDLK_w && !engine_mode) {
                player->move("up");
            }
            if (event.key.keysym.sym == SDLK_a && !engine_mode) {
                player->move("left");
                current_idle_animation = "player_idle_left";
            }
            if (event.key.keysym.sym == SDLK_s && !engine_mode) {
                player->move("down");
            }
            if (event.key.keysym.sym == SDLK_d && !engine_mode) {
                player->move("right");
                current_idle_animation = "player_idle_right";
            }
            if (event.key.keysym.sym == SDLK_x) {
                engine_mode = !engine_mode;
                map_reset();
            }
            if (event.key.keysym.sym == SDLK_y && !engine_mode) {
                reload_map(walls,renderer);
                //do the map reset here.
                
            }
            if (event.key.keysym.sym == SDLK_l && engine_mode) {
                destroy_map(walls);
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
                direction_stream.push('W');
            }
            if (event.key.keysym.sym == SDLK_a && engine_mode) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->x += camera_offset;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.x += camera_offset;
                }
                player->rect.x += camera_offset;
                direction_stream.push('A');
            }
            if (event.key.keysym.sym == SDLK_s && engine_mode) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->y -= camera_offset;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.y -= camera_offset;
                }
                player->rect.y -= camera_offset;
                direction_stream.push('S');
            }
            if (event.key.keysym.sym == SDLK_d && engine_mode) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->x -= camera_offset;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.x -= camera_offset;
                }
                player->rect.x -= camera_offset;
                direction_stream.push('D');
            }
            if (event.key.keysym.sym == SDLK_r && engine_mode) {
                save_map(walls,0);
            }
            break;

        case SDL_QUIT:
            map_reset();
            save_map(walls, 0);
            is_game_running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            int x = event.motion.x;
            int y = event.motion.y;

            SDL_Point mouse_position = { x, y };

            if (engine_mode) {
                if (event.button.button == SDL_BUTTON_LEFT) { // Right mouse button
                    for (int i = 0; i < level_grid.size(); i++) {
                        if (SDL_PointInRect(&mouse_position, level_grid[i])) {
                            place_wall_pixels(walls,level_grid[i]->x, level_grid[i]->y,renderer);
                        }
                    }
                }
                else if (event.button.button == SDL_BUTTON_RIGHT) { // Left mouse button
                    for (int i = 0; i < level_grid.size(); i++) {
                        if (SDL_PointInRect(&mouse_position, level_grid[i])) {
                            //delete that specific wall of location x,y
                            remove_wall_pixels(walls,level_grid[i]->x, level_grid[i]->y);
                        }
                    }
                }
            }

            break;

        }
    }
}


void draw_text_init(){
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

// Generated by ChatGPT.
void remove_wall_pixels(int x, int y) {
    auto it = std::remove_if(walls.begin(), walls.end(), [x, y](const Wall& wall) {
        return wall.rect.x == x && wall.rect.y == y;
        });


    if (it != walls.end()) {
        walls.erase(it, walls.end());
    }
}



void map_reset() {
    if (direction_stream.empty() == true) { std::cout << "STACK EMPTY!\n"; return; }
    // do the reset logic here !!!!
    while (direction_stream.empty() == false) {
        char tmp = direction_stream.top();
        // camera movement logic
        if (tmp == 'W') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->y -= camera_offset;

            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.y -= camera_offset;
            }
            player->rect.y -= camera_offset;
        }
        else if (tmp == 'A') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x -= camera_offset;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.x -= camera_offset;
            }
            player->rect.x -= camera_offset;
        }
        else if (tmp == 'S') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->y += camera_offset;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.y += camera_offset;
            }
            player->rect.y += camera_offset;
        }
        else if (tmp == 'D') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x += camera_offset;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.x += camera_offset;
            }
            player->rect.x += camera_offset;
        }
        direction_stream.pop();
    }
}