#define SPRITE_SIZE 64


#include <SDL.h>
#include <SDL_image.h>

#include <string>

SDL_Window* window;
SDL_Renderer* renderer;

class Player {
public:
    SDL_Rect rect = { 0, 0, SPRITE_SIZE, SPRITE_SIZE };
    SDL_Texture* texture;
    int destination;
    bool can_move_right;
    Player() {
        destination = 0;
        can_move_right = false;
        texture = IMG_LoadTexture(renderer, "data/sprite_player.png");
    }
    void move(const std::string& direction) {
        if (direction == "right") {
            destination = rect.x + SPRITE_SIZE;
            can_move_right = true;
        }
        else if (direction == "left") {
            rect.x -= SPRITE_SIZE;
        }
        else if (direction == "up") {
            rect.y -= SPRITE_SIZE;
        }
        else if (direction == "down") {
            rect.y += SPRITE_SIZE;
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
        player->move_constant_right();
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