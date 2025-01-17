#define SPRITE_SIZE 64
#define WINDOW_RES_X 1280
#define WINDOW_RES_Y 720
#define ANGKOR_WAT "data/map_config_angkor_wat.level"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

#include "grid_manager.h"
#include "map_manager.h"
#include "player.h"
#include "wall.h"
#include "stone.h"
#include "back_wall.h"

// Sprite names for display in the engine mode.
// This corresponds to the below level_item enum.
std::vector<std::string> sprite_names = {
    "Player",
    "Wall",
    "Back Wall"
};

// level_item::PLAYER starts from 0.
enum level_item {
    PLAYER,
    WALL,
    BACK_WALL
};

int current_level_item_to_be_placed = level_item::WALL;


SDL_Window* window;
SDL_Renderer* renderer;
Player* player;

std::vector<Wall> walls;
std::vector<Stone> stones;
std::vector<Back_Wall> back_walls;
std::vector<SDL_FRect*> level_grid;
std::stack<char> direction_stream;

TTF_Font* font;
SDL_Surface* text_surface;
SDL_Texture* text_texture;
float camera_speed = 2;
std::vector<SDL_Texture*> animation_player_idle_list;
std::vector<SDL_Texture*> animation_player_walk_list;
std::vector<SDL_Texture*> animation_stone_move_list;
 
bool is_game_running = true;

const Uint8* key_state = SDL_GetKeyboardState(nullptr);
char last_key_held = 'w';
bool is_player_moving = false;
float change = 0.1f;

// engine_mode = false is game_mode. ;)
bool engine_mode = false;

void draw();
void draw_walls();
void draw_back_walls();
void draw_stones();
void place_stone(float x, float y);
void process_input();

void draw_text(std::string, float, float, SDL_Color*);
void draw_text_init();
void init_animation();
void play_animation(const std::string&);
void update_animation();
void remove_walls(float, float);
void map_reset();
bool is_wall_on_right_side();
bool is_wall_on_left_side();
bool is_wall_on_up_side();
bool is_wall_on_down_side();
void remove_back_walls(int x, int y);



SDL_Color color_white = { 255, 255, 255, 255 };

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP , &window, &renderer);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    player = new Player(renderer, 3, 3);

    // Load the game map.
    reload_map(walls, back_walls, renderer);
    create_level_grid_rects(level_grid);
    draw_text_init();
    init_animation();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Game Loop.
    while (is_game_running) {
        draw();
        process_input();
    }
    
    // Unload
    IMG_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();

    return 0;
}

std::string current_animation = "player_idle_right";
int animation_index = 0;
float animation_speed = 300;
void draw() {

    draw_walls();
    draw_back_walls();

    // Call this only inside !engine_mode.
    play_animation(current_animation);

    if (engine_mode) {
        draw_text("(X)    ENGINE MODE", 1000, 0, &color_white);

        show_grid(level_grid, renderer);

        std::string position = std::to_string((int)player->rect.x) + "," + std::to_string((int)player->rect.y);
        draw_text(position, player->rect.x, player->rect.y, &color_white);

        draw_text("(R)    SAVE",         1000, 20,  &color_white);
        draw_text("(L)    DESTROY",      1000, 40,  &color_white);
        draw_text("(Y)    RELOAD",       1000, 60,  &color_white);
        draw_text("(MB1)  PLACE ENTITY", 1000, 80, &color_white);
        draw_text("(Q/E)  SELECT ENTITY: " + sprite_names[current_level_item_to_be_placed], 1000, 100, &color_white);
        draw_text("(WASD) MOVE CAMERA",  1000, 120, &color_white);
    }
    
    if (!engine_mode) {
        draw_text("(X)   GAME MODE", 1000, 0, &color_white);
    }


    SDL_RenderPresent(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    update_animation();
}

// The global variable current_animation (the main variable controlling which animation to play)
// is basically defined here. Like these are the choices.

void play_animation(const std::string& animation_name) {
    if (animation_name == "player_idle_right") {
        if (!engine_mode) {
            if (animation_index >= animation_player_idle_list.size()) {
                animation_index = 0;
            }
            player->texture = animation_player_idle_list[animation_index];
            SDL_RenderCopyF(renderer, player->texture, 0, &player->rect);
        }
        else {
            SDL_RenderCopyF(renderer, player->texture, 0, &player->rect);
        }
    }

    if (animation_name == "player_idle_left") {
        if (!engine_mode) {
            if (animation_index >= animation_player_idle_list.size()) {
                animation_index = 0;
            }
            player->texture = animation_player_idle_list[animation_index];
            SDL_RenderCopyExF(renderer, player->texture, 0, &player->rect, 0, 0, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopyExF(renderer, player->texture, 0, &player->rect, 0, 0, SDL_FLIP_HORIZONTAL);
        }
    }

    if (animation_name == "player_walk_right") {
        if (!engine_mode) {
            if (animation_index >= animation_player_walk_list.size()) {
                animation_index = 0;
            }
            player->texture = animation_player_walk_list[animation_index];
            SDL_RenderCopyF(renderer, player->texture, 0, &player->rect);
        }
        else {
            SDL_RenderCopyF(renderer, player->texture, 0, &player->rect);
        }
    }

    if (animation_name == "player_walk_left") {
        if (!engine_mode) {
            if (animation_index >= animation_player_walk_list.size()) {
                animation_index = 0;
            }
            player->texture = animation_player_walk_list[animation_index];
            SDL_RenderCopyExF(renderer, player->texture, 0, &player->rect, 0, 0, SDL_FLIP_HORIZONTAL);
        }
        else {
            SDL_RenderCopyExF(renderer, player->texture, 0, &player->rect,0, 0, SDL_FLIP_HORIZONTAL);
        }
    }
}

// Maybe elapased time or frame calculations can be used in the future.
float new_animation_speed = animation_speed;
void update_animation() {
    if (new_animation_speed <= 0) {
        new_animation_speed = animation_speed;
        animation_index++;
    }
    new_animation_speed -= 0.5f;
}

void draw_walls() {
    for (int i = 0; i < walls.size(); i++) {
        SDL_RenderCopy(renderer, walls[i].texture, 0, &walls[i].rect);
    }
}

// we should make a generic function which takes in a level_item_type
// and draws it. same for placing them.
void draw_back_walls() {
    for (int i = 0; i < back_walls.size(); i++) {
        SDL_RenderCopy(renderer, back_walls[i].texture, 0, &back_walls[i].rect);
    }
}

void draw_stones() {
    for (int i = 0; i < stones.size(); i++) {
        SDL_RenderCopyF(renderer, stones[i].texture, 0, &stones[i].rect);
    }
}



void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                is_game_running = false;
                map_reset();
                save_map(walls, back_walls, 0);
            } break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_game_running = false;
                }
                if (event.key.keysym.sym == SDLK_x) {
                    engine_mode = !engine_mode;
                    map_reset();
                }
                if (event.key.keysym.sym == SDLK_y && !engine_mode) {
                    reload_map(walls, back_walls, renderer);
                }
                if (event.key.keysym.sym == SDLK_l && engine_mode) {
                    destroy_map(walls);
                }
                if (event.key.keysym.sym == SDLK_e && engine_mode) {
                    current_level_item_to_be_placed++;
                    if (current_level_item_to_be_placed == sprite_names.size()) {
                        current_level_item_to_be_placed = 1;
                    }
                }
                if (event.key.keysym.sym == SDLK_q && engine_mode) {
                    current_level_item_to_be_placed--;
                    if (current_level_item_to_be_placed == 0) {
                        current_level_item_to_be_placed = sprite_names.size() - 1;
                    }
                }
            } break;

            case SDL_MOUSEBUTTONDOWN: {
                float x = event.motion.x;
                float y = event.motion.y;
                SDL_FPoint mouse_position = { x, y };

                if (engine_mode) {
                    if (event.button.button == SDL_BUTTON_LEFT) {

                        for (int i = 0; i < level_grid.size(); i++) {
                            if (SDL_PointInFRect(&mouse_position, level_grid[i])) {
                                // Wall Placement during Engine Mode.
                                if (current_level_item_to_be_placed == level_item::WALL) {
                                    place_wall(level_grid[i]->x, level_grid[i]->y, renderer, walls);
                                }
                                // Back Wall Placement.
                                if (current_level_item_to_be_placed == level_item::BACK_WALL) {
                                    place_back_wall(level_grid[i]->x, level_grid[i]->y, renderer, back_walls);
                                }
                            }
                        }

                    }
                    else if (event.button.button == SDL_BUTTON_RIGHT) {
                        for (int i = 0; i < level_grid.size(); i++) {
                            if (SDL_PointInFRect(&mouse_position, level_grid[i])) {
                                //delete that specific wall of location x,y
                                if (current_level_item_to_be_placed == level_item::WALL) {
                                    remove_walls(level_grid[i]->x, level_grid[i]->y);
                                }
                                if (current_level_item_to_be_placed == level_item::BACK_WALL) {
                                    remove_back_walls(level_grid[i]->x, level_grid[i]->y);
                                }
                            }
                        }
                    }

                }

            } break;
        }
    }
    
    if (engine_mode) {
        signed int step = 1;
        const Uint8* keyStates = SDL_GetKeyboardState(NULL);
        if (keyStates[SDL_SCANCODE_W]) {
            while (step) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->y += camera_speed;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.y += camera_speed;
                }
                for (int i = 0; i < back_walls.size(); i++) {
                    back_walls[i].rect.y += camera_speed;
                }
                player->rect.y += camera_speed;
                direction_stream.push('W');
                step--;
            }
        }else if (keyStates[SDL_SCANCODE_A]) {
            while (step) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->x += camera_speed;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.x += camera_speed;
                }
                for (int i = 0; i < back_walls.size(); i++) {
                    back_walls[i].rect.x += camera_speed;
                }
                player->rect.x += camera_speed;
                direction_stream.push('A');
                step--;
            }
        }else if (keyStates[SDL_SCANCODE_S]) {
            while (step) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->y -= camera_speed;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.y -= camera_speed;
                }
                for (int i = 0; i < back_walls.size(); i++) {
                    back_walls[i].rect.y -= camera_speed;
                }
                player->rect.y -= camera_speed;
                direction_stream.push('S');
                step--;
            }
        }if (keyStates[SDL_SCANCODE_D]) {
            while (step) {
                for (int i = 0; i < level_grid.size(); i++) {
                    level_grid[i]->x -= camera_speed;
                }
                for (int i = 0; i < walls.size(); i++) {
                    walls[i].rect.x -= camera_speed;
                }
                for (int i = 0; i < back_walls.size(); i++) {
                    back_walls[i].rect.x -= camera_speed;
                }
                player->rect.x -= camera_speed;
                direction_stream.push('D');
                step--;
            }
        }
    }

    else if (!is_player_moving && !engine_mode) {
        if (!is_wall_on_right_side() && !key_state[SDL_SCANCODE_W] && !key_state[SDL_SCANCODE_A] &&
            !key_state[SDL_SCANCODE_S] && key_state[SDL_SCANCODE_D]) 
        {
            player->rect.x++;
            last_key_held = 'd';
            is_player_moving = true;
            current_animation = "player_walk_right";
        }

        if (!is_wall_on_left_side() && !key_state[SDL_SCANCODE_W] && key_state[SDL_SCANCODE_A] &&
            !key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {    
            player->rect.x--;
            last_key_held = 'a';
            is_player_moving = true;
            current_animation = "player_walk_left";
        }

        if (!is_wall_on_up_side() && key_state[SDL_SCANCODE_W] && !key_state[SDL_SCANCODE_A] &&
            !key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {
            player->rect.y--;
            last_key_held = 'w';
            is_player_moving = true;
        }

        if (!is_wall_on_down_side() && !key_state[SDL_SCANCODE_W] && !key_state[SDL_SCANCODE_A] &&
            key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {
            player->rect.y++;
            last_key_held = 's';
            is_player_moving = true;
        }

        if (key_state[SDL_SCANCODE_F]) {
            for (int i = 0; i < stones.size(); i++) {
                current_animation = "stone_move_right";
                stones[i].rect.x += 0.1;
            }
        }
    }

    if (!engine_mode) {
        switch (last_key_held) {
            case 'w': {
                if ((int)player->rect.y % 64 == 0) {
                    is_player_moving = false;
                }
                if ((int)player->rect.y % 64 != 0) {
                    player->rect.y -= change;
                }
            } break;

            case 'a': {
                if ((int)player->rect.x % 64 == 0) {
                    is_player_moving = false;
                    current_animation = "player_idle_left";
                }
                if ((int)player->rect.x % 64 != 0) {
                    player->rect.x -= change;
                }
            } break;

            case 's': {
                if ((int)player->rect.y % 64 == 0) {
                    is_player_moving = false;
                }
                if ((int)player->rect.y % 64 != 0) {
                    player->rect.y += change;
                }
            } break;

            case 'd': {
                if ((int)player->rect.x % 64 == 0) {
                    is_player_moving = false;
                    current_animation = "player_idle_right";
                }
                if ((int)player->rect.x % 64 != 0) {
                    player->rect.x += change;
                }
            } break;
        }
    }
}

void draw_text_init() { 
    font = TTF_OpenFont("data/font/consolas_bold.ttf", 20);
}

void draw_text(std::string text, float x, float y, SDL_Color* color) {
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
    SDL_FRect text_rect = { x, y, 10 * text.length(), 23 };
    SDL_RenderCopyF(renderer, last_text_texture, 0, &text_rect);
}

void init_animation() {
    // Player Idle Animation.
    for (int i = 0; i <= 8; i++) {
        animation_player_idle_list.push_back(IMG_LoadTexture(renderer, ("data/animation/player_idle/" + std::to_string(i) + ".png").c_str()));
    }
    // Player Walk Animation.
    for (int i = 0; i <= 3; i++) {
        animation_player_walk_list.push_back(IMG_LoadTexture(renderer, ("data/animation/player_walk/" + std::to_string(i) + ".png").c_str()));
    }
}


void remove_walls(float x, float y) {
    auto it = std::remove_if(walls.begin(), walls.end(), [x, y](const Wall& wall) {
        return wall.rect.x == x && wall.rect.y == y;
    });
    if (it != walls.end()) {
        walls.erase(it, walls.end());
    }
}

void remove_back_walls(int x, int y) {
    auto it = std::remove_if(back_walls.begin(), back_walls.end(), [x, y](const Back_Wall& back_wall) {
        return back_wall.rect.x == x && back_wall.rect.y == y;
    });

    if (it != back_walls.end()) {
        back_walls.erase(it, back_walls.end());
    }
}

void map_reset() {
    if (direction_stream.empty() == true) {
        return;
    }
    // do the reset logic here !!!!
    while (direction_stream.empty() == false) {
        char tmp = direction_stream.top();
        // camera movement logic
        if (tmp == 'W') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->y -= camera_speed;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.y -= camera_speed;
            }
            for (int i = 0; i < back_walls.size(); i++) {
                back_walls[i].rect.y -= camera_speed;
            }
            player->rect.y -= camera_speed;
        }
        else if (tmp == 'A') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x -= camera_speed;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.x -= camera_speed;
            }
            for (int i = 0; i < back_walls.size(); i++) {
                back_walls[i].rect.x -= camera_speed;
            }
            player->rect.x -= camera_speed;
        }
        else if (tmp == 'S') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->y += camera_speed;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.y += camera_speed;
            }
            for (int i = 0; i < back_walls.size(); i++) {
                back_walls[i].rect.y += camera_speed;
            }
            player->rect.y += camera_speed;
        }
        else if (tmp == 'D') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x += camera_speed;
            }
            for (int i = 0; i < walls.size(); i++) {
                walls[i].rect.x += camera_speed;
            }
            for (int i = 0; i < back_walls.size(); i++) {
                back_walls[i].rect.x += camera_speed;
            }
            player->rect.x += camera_speed;
        }
        direction_stream.pop();
    }
}

// Collison detection between player and wall.
bool is_wall_on_right_side() {
    SDL_Point right_tile_point = { player->rect.x + (SPRITE_SIZE), player->rect.y };
    for (int i = 0; i < walls.size(); i++) {
        if (SDL_PointInRect(&right_tile_point, &walls[i].rect)) {
            return true;
        }
    }
    return false;
}
bool is_wall_on_left_side() {
    SDL_Point left_tile_point = { player->rect.x - 1, player->rect.y };
    for (int i = 0; i < walls.size(); i++) {
        if (SDL_PointInRect(&left_tile_point, &walls[i].rect)) {
            return true;
        }
    }
    return false;
}
bool is_wall_on_up_side() {
    SDL_Point up_tile_point = { player->rect.x, player->rect.y - 1 };
    for (int i = 0; i < walls.size(); i++) {
        if (SDL_PointInRect(&up_tile_point, &walls[i].rect)) {
            return true;
        }
    }
    return false;
}
bool is_wall_on_down_side() {
    SDL_Point down_tile_point = { player->rect.x, player->rect.y + SPRITE_SIZE };
    for (int i = 0; i < walls.size(); i++) {
        if (SDL_PointInRect(&down_tile_point, &walls[i].rect)) {
            return true;
        }
    }
    return false;
}

void place_stone(float x, float y) {
    // remove this type argument from the class constructor.
    stones.push_back(Stone(x, y, "stone", renderer));
}