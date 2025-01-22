#define SPRITE_SIZE 64

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
#include "entity.h"

// Sprite names for display in the engine mode.
// This corresponds to the below level_item enum and level_sprites.
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
std::vector<SDL_Texture*> level_sprites;
int current_level_item_to_be_placed = level_item::WALL;

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;
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
// engine_mode = false is game_mode. ;)
bool engine_mode = false;
// Side panel starting_x variable.
int starting_x = WINDOW_RES_X - 400;
bool is_mouse_held = false;
bool is_over_side_panel = false;
std::vector<Entity*> entities;

void draw();
void process_input();
void draw_text(std::string, float, float, SDL_Color*);
void draw_text_init();
void init_animation();
void play_animation(const std::string&);
void update_animation();
void map_reset();
bool is_wall_on_right_side();
bool is_wall_on_left_side();
bool is_wall_on_up_side();
bool is_wall_on_down_side();
void draw_engine_side_panel();
void draw_entity_below_mouse();
void draw_entities();


SDL_Color color_white = { 255, 255, 255, 255 };

#define DEBUG 1

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);



#if DEBUG
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN, &window, &renderer);
#else
    SDL_CreateWindowAndRenderer(WINDOW_RES_X, WINDOW_RES_Y, SDL_WINDOW_SHOWN | SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer);
#endif




    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    player = new Player(renderer, 3, 3);

    // Load the game map.
    reload_map(entities, renderer);
    create_level_grid_rects(level_grid);
    draw_text_init();
    init_animation();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    level_sprites.push_back(IMG_LoadTexture(renderer, "data/sprite_player.png"));
    level_sprites.push_back(IMG_LoadTexture(renderer, "data/sprite_wall.png"));
    level_sprites.push_back(IMG_LoadTexture(renderer, "data/sprite_back_wall.png"));

    int start_time = SDL_GetTicks();
    int frame_count = 0;
    int fps = 0;
    while (is_game_running) {                   // Game / Application Loop.
        int current_time = SDL_GetTicks();
        int elapsed_time = (current_time - start_time) / 1000;

        
        draw();
        process_input();
        

        frame_count++;
        if (elapsed_time >= 1.0f) {             // if 1 second has elapsed, how many frames have been drawn?
            fps = frame_count;                  // fps stores the number of frames drawn in the 1 second interval.
            frame_count = 0;                    // frame_count sets to 9.
            start_time = SDL_GetTicks();        // we start the start timer again, for comparison with the next frame.
        }

        draw_text("FPS: " + std::to_string(fps), 0, 0, &color_white);
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

    //draw_walls();
    //draw_back_walls();

    

    draw_entities();

    // Call this only inside !engine_mode.
    play_animation(current_animation);

    if (engine_mode) {
        show_grid(level_grid, renderer);
    }

    draw_engine_side_panel();
    
    if (engine_mode) {

        std::string position = std::to_string((int)player->rect.x) + "," + std::to_string((int)player->rect.y);
        draw_text(position, player->rect.x, player->rect.y, &color_white);

        draw_entity_below_mouse();
        
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

void draw_entities() {
    for (int i = 0; i < entities.size(); i++) {
        SDL_RenderCopyF(renderer, entities[i]->texture, 0, &entities[i]->rect);
    }
}


void process_input() {

    int mouse_x, mouse_y;
    int button_state = SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_FPoint mouse_position = SDL_FPoint{ (float)mouse_x, (float)mouse_y };

    if (engine_mode) {
        if (!is_over_side_panel && is_mouse_held) {
            if (button_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                for (int i = 0; i < level_grid.size(); i++) {
                    if (SDL_PointInFRect(&mouse_position, level_grid[i])) {
                        
                        if (current_level_item_to_be_placed == level_item::WALL) {
                            place_entity(level_grid[i]->x, level_grid[i]->y, "wall", entities, renderer);
                        }
                        
                        if (current_level_item_to_be_placed == level_item::BACK_WALL) {
                            place_entity(level_grid[i]->x, level_grid[i]->y, "back_wall", entities, renderer);
                        }
                    }
                }
            }
            else if (button_state & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                for (int i = 0; i < level_grid.size(); i++) {
                    if (SDL_PointInFRect(&mouse_position, level_grid[i])) {
                        remove_entity(level_grid[i]->x, level_grid[i]->y, entities);
                    }
                }
            }
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                is_game_running = false;
                map_reset();
                save_map(entities, 0);
            } break;

            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_game_running = false;
                }
                if (event.key.keysym.sym == SDLK_x) {
                    engine_mode = !engine_mode;
                    map_reset();
                    save_map(entities, 0);
                }
                if (event.key.keysym.sym == SDLK_r && !engine_mode) {
                    save_map(entities, 0);
                }
                if (event.key.keysym.sym == SDLK_y && !engine_mode) {
                    reload_map(entities, renderer);
                }
                if (event.key.keysym.sym == SDLK_l && engine_mode) {
                    
                    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
                    // THIS NEEDS TO BE IMPLEMENTED ASAP!!
                    //destroy_map(walls, back_walls);
                    // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
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

            case SDL_MOUSEBUTTONUP: {
                is_mouse_held = false;
            } break;

            case SDL_MOUSEBUTTONDOWN: {
                is_mouse_held = true;
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
                for (int i = 0; i < entities.size(); i++) {
                    entities[i]->rect.y += camera_speed;
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
                for (int i = 0; i < entities.size(); i++) {
                    entities[i]->rect.x += camera_speed;
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
                for (int i = 0; i < entities.size(); i++) {
                    entities[i]->rect.y -= camera_speed;
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
                for (int i = 0; i < entities.size(); i++) {
                    entities[i]->rect.x -= camera_speed;
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
            player->rect.x += player->speed;
            last_key_held = 'd';
            is_player_moving = true;
            current_animation = "player_walk_right";
        }

        if (!is_wall_on_left_side() && !key_state[SDL_SCANCODE_W] && key_state[SDL_SCANCODE_A] &&
            !key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {    
            player->rect.x -= player->speed;
            last_key_held = 'a';
            is_player_moving = true;
            current_animation = "player_walk_left";
        }

        if (!is_wall_on_up_side() && key_state[SDL_SCANCODE_W] && !key_state[SDL_SCANCODE_A] &&
            !key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {
            player->rect.y -= player->speed;
            last_key_held = 'w';
            is_player_moving = true;
        }

        if (!is_wall_on_down_side() && !key_state[SDL_SCANCODE_W] && !key_state[SDL_SCANCODE_A] &&
            key_state[SDL_SCANCODE_S] && !key_state[SDL_SCANCODE_D]) 
        {
            player->rect.y += player->speed;
            last_key_held = 's';
            is_player_moving = true;
        }
    }

    if (!engine_mode) {
        switch (last_key_held) {
            case 'w': {
                if ((int)player->rect.y % 64 == 0) {
                    is_player_moving = false;
                }
                if ((int)player->rect.y % 64 != 0) {
                    player->rect.y -= player->speed;
                }
            } break;

            case 'a': {
                if ((int)player->rect.x % 64 == 0) {
                    is_player_moving = false;
                    current_animation = "player_idle_left";
                }
                if ((int)player->rect.x % 64 != 0) {
                    player->rect.x -= player->speed;
                }
            } break;

            case 's': {
                if ((int)player->rect.y % 64 == 0) {
                    is_player_moving = false;
                }
                if ((int)player->rect.y % 64 != 0) {
                    player->rect.y += player->speed;
                }
            } break;

            case 'd': {
                if ((int)player->rect.x % 64 == 0) {
                    is_player_moving = false;
                    current_animation = "player_idle_right";
                }
                if ((int)player->rect.x % 64 != 0) {
                    player->rect.x += player->speed;
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

// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// // xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// 
// 
// THIS NEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEDS TO BE IMPLEMENTED ASAP!!!!
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
// xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx


//void remove_walls(float x, float y) {
//    auto it = std::remove_if(walls.begin(), walls.end(), [x, y](const Wall& wall) {
//        return wall.rect.x == x && wall.rect.y == y;
//    });
//    if (it != walls.end()) {
//        walls.erase(it, walls.end());
//    }
//}
//
//void remove_back_walls(int x, int y) {
//    auto it = std::remove_if(back_walls.begin(), back_walls.end(), [x, y](const Back_Wall& back_wall) {
//        return back_wall.rect.x == x && back_wall.rect.y == y;
//    });
//
//    if (it != back_walls.end()) {
//        back_walls.erase(it, back_walls.end());
//    }
//}



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
            // We need to do this for all the entities automatically.
            // Another reason for creating a list of all entities.
            
            for (int i = 0; i < entities.size(); i++) {
                entities[i]->rect.y -= camera_speed;
            }
            player->rect.y -= camera_speed;
        }
        else if (tmp == 'A') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x -= camera_speed;
            }
            for (int i = 0; i < entities.size(); i++) {
                entities[i]->rect.x -= camera_speed;
            }
            player->rect.x -= camera_speed;
        }
        else if (tmp == 'S') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->y += camera_speed;
            }
            for (int i = 0; i < entities.size(); i++) {
                entities[i]->rect.y += camera_speed;
            }
            player->rect.y += camera_speed;
        }
        else if (tmp == 'D') {
            for (int i = 0; i < level_grid.size(); i++) {
                level_grid[i]->x += camera_speed;
            }
            for (int i = 0; i < entities.size(); i++) {
                entities[i]->rect.x += camera_speed;
            }
            player->rect.x += camera_speed;
        }
        direction_stream.pop();
    }
}

// Collison detection between player and wall.
bool is_wall_on_right_side() {
    SDL_FPoint right_tile_point = { player->rect.x + (SPRITE_SIZE), player->rect.y };
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->type == "wall") {
            if (SDL_PointInFRect(&right_tile_point, &entities[i]->rect)) {
                return true;
            }
        }
    }
    return false;
}
bool is_wall_on_left_side() {
    SDL_FPoint left_tile_point = { player->rect.x - 1, player->rect.y };
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->type == "wall") {
            if (SDL_PointInFRect(&left_tile_point, &entities[i]->rect)) {
                return true;
            }
        }
    }
    return false;
}
bool is_wall_on_up_side() {
    SDL_FPoint up_tile_point = { player->rect.x, player->rect.y - 1 };
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->type == "wall") {
            if (SDL_PointInFRect(&up_tile_point, &entities[i]->rect)) {
                return true;
            }
        }
    }
    return false;
}
bool is_wall_on_down_side() {
    SDL_FPoint down_tile_point = { player->rect.x, player->rect.y + SPRITE_SIZE };
    for (int i = 0; i < entities.size(); i++) {
        if (entities[i]->type == "wall") {
            if (SDL_PointInFRect(&down_tile_point, &entities[i]->rect)) {
                return true;
            }
        }
    }
    return false;
}

void draw_entity_below_mouse() {
    
    int mouse_x;
    int mouse_y;

    SDL_GetMouseState(&mouse_x, &mouse_y);
    
    // There is another scoped offset variable doing the same thing in draw_engine_side_panel() function.
    // So we need to change both of them.
    int offset = 5;
    if (mouse_x < starting_x - offset) {
        SDL_Rect rect_below_mouse = { mouse_x - (SPRITE_SIZE / 2), mouse_y - (SPRITE_SIZE / 2), SPRITE_SIZE, SPRITE_SIZE };
        // Set hte transparency value to some number between 0 and 255.
        SDL_SetTextureAlphaMod(level_sprites[current_level_item_to_be_placed], 100);
        SDL_RenderCopy(renderer, level_sprites[current_level_item_to_be_placed], 0, &rect_below_mouse);
        // Reset the transparency value of the sprite.
        SDL_SetTextureAlphaMod(level_sprites[current_level_item_to_be_placed], 255);
    }
}

// Needs extreme amounts of optimisations like not recreating
// the side_panel_rect each time every frame.
void draw_engine_side_panel() {
    
    

    int starting_y = 0;
    int width = WINDOW_RES_X - starting_x;
    int height = WINDOW_RES_Y - starting_y;


    if (engine_mode) {
        
        
    
        // ---------------------------------------------------------
            SDL_Rect side_panel_rect = {
                starting_x, starting_y, 
                width, height
            };


            SDL_SetRenderDrawColor(renderer, 23, 29, 27, 255);
            SDL_RenderFillRect(renderer, &side_panel_rect);
            SDL_RenderDrawRect(renderer, &side_panel_rect);
        // ---------------------------------------------------------


        // ---------------------------------------------------------
            SDL_Rect current_entity_sprite_rect = {
                starting_x + 70, starting_y + 145,
                SPRITE_SIZE, SPRITE_SIZE
            };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            //SDL_RenderFillRect(renderer, &current_entity_sprite_rect);
            SDL_RenderCopy(renderer, level_sprites[current_level_item_to_be_placed], 0, &current_entity_sprite_rect);
            SDL_RenderDrawRect(renderer, &current_entity_sprite_rect);
        // ---------------------------------------------------------




        // ---------------------------------------------------------
            draw_text("(X)    ENGINE MODE",                                                     starting_x, 0,   &color_white);
            draw_text("(R)    SAVE",                                                            starting_x, 20,  &color_white);
            draw_text("(L)    DESTROY",                                                         starting_x, 40,  &color_white);
            draw_text("(Y)    RELOAD",                                                          starting_x, 60,  &color_white);
            draw_text("(WASD) MOVE CAMERA",                                                     starting_x, 80,  &color_white);
            draw_text("(MB1)  PLACE ENTITY",                                                    starting_x, 100, &color_white);
            draw_text("(Q/E)  SELECT ENTITY: " + sprite_names[current_level_item_to_be_placed], starting_x, 120, &color_white);
            draw_text(ANGKOR_WAT, starting_x, 220, &color_white);
        // ---------------------------------------------------------


        // ---------------------------------------------------------
            
            int mouse_x;
            int mouse_y;

            SDL_GetMouseState(&mouse_x, &mouse_y);
            
            SDL_Cursor* panel_resize_cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
            SDL_Cursor* default_cursor      = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

            int cursor_change_offset = 5;
            if (mouse_x >= starting_x - cursor_change_offset && mouse_x <= starting_x + cursor_change_offset) {
                SDL_SetCursor(panel_resize_cursor);
            } else {
                SDL_SetCursor(default_cursor);
            }

            int side_panel_motion_offset = 50;
            if (is_mouse_held && mouse_x >= starting_x - side_panel_motion_offset && mouse_x <= starting_x + side_panel_motion_offset) {
                
                starting_x = mouse_x;
            }

            int side_panel_over_offset = 15;
            if (mouse_x > starting_x - side_panel_over_offset) {
                is_over_side_panel = true;
            } else {
                is_over_side_panel = false;
            }
            

        // ---------------------------------------------------------

    }



    if (!engine_mode) {
        draw_text("(X)   GAME MODE", starting_x, 0, &color_white);
    }
}