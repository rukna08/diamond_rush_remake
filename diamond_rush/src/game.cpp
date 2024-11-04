


#include <SDL.h>



SDL_Window* window;



void draw();
void process_input();

bool is_game_running = true;



int main(int argc, char* argv[]) {



    SDL_Init(SDL_INIT_VIDEO);



    window = SDL_CreateWindow("Diamond Rush Remake", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN);
    
    while (is_game_running) {

        draw();

        process_input();

    }
    



    return 0;



}



void draw() {
    

    

    SDL_FillRect(SDL_GetWindowSurface(window), 0, SDL_MapRGB(SDL_GetWindowSurface(window)->format, 255, 0, 255));
    SDL_UpdateWindowSurface(window);



}



void process_input() {
    
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        

        switch (event.type) {



            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    is_game_running = false;
                }
            break;



        }


    }





}