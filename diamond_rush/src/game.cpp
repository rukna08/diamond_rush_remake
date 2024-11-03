


#include <SDL.h>



SDL_Window* window;



int main(int argc, char* argv[]) {



    SDL_Init(SDL_INIT_VIDEO);



    window = SDL_CreateWindow("Diamond Rush Remake", 0, 0, 1920, 1080, SDL_WINDOW_FULLSCREEN);
    
    
    
    SDL_FillRect(SDL_GetWindowSurface(window), 0, SDL_MapRGB(SDL_GetWindowSurface(window)->format, 255, 0, 255));
    SDL_UpdateWindowSurface(window);



    SDL_Delay(3000);



    return 0;



}