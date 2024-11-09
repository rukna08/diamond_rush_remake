#define SPRITE_SIZE 64


#include <SDL.h>
#include <SDL_image.h>

#include <string>

class Player;

SDL_Window* window;
SDL_Renderer* renderer;
Player* player;

class Player {
public:
    SDL_Rect rect = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    SDL_Texture* texture;
    int destination;
    bool can_move_right;
    bool can_move_left;
    bool can_move_up;
    bool can_move_down;
    Player() {
        destination = 0;
        can_move_right = false;
        can_move_left = false;
        can_move_up = false;
        can_move_down = false;
        texture = IMG_LoadTexture(renderer, "data/sprite_player.png");
    }
    void move(const std::string& direction) {
        if (!can_move_up && !can_move_down && !can_move_left && !can_move_right) {
            if (direction == "right") {
                destination = rect.x + SPRITE_SIZE;
                can_move_right = true;
            } else if (direction == "left") {
                destination = rect.x - SPRITE_SIZE;
                can_move_left = true;
            } else if (direction == "up") {
                destination = rect.y - SPRITE_SIZE;
                can_move_up = true;
            } else if (direction == "down") {
                destination = rect.y + SPRITE_SIZE;
                can_move_down = true;
            }
        }
    }
    void move_constant_right() {
        if (rect.x == destination) {
            can_move_right = false;
        }
        if (can_move_right && rect.x < destination) {
            rect.x++;
        }
    }
    void move_constant_left() {
        if (rect.x == destination) {
            can_move_left = false;
        }
        if (can_move_left && rect.x > destination) {
            rect.x--;
        }
    }
    void move_constant_down() {
        if (rect.y == destination) {
            can_move_down = false;
        }
        if (can_move_down && rect.y < destination) {
            rect.y++;
        }
    }
    void move_constant_up() {
        if (rect.y == destination) {
            can_move_up = false;
        }
        if (can_move_up && rect.y > destination) {
            rect.y--;
        }
    }
};



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