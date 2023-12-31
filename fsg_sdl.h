#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct sdl_base {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GameController** gamepads;
    SDL_Event event;
    int run;
    uint16_t w, h;
    const char* title;
} sdl_base;

sdl_base* sdl_create(const char* title, const size_t w, const size_t h);
void sdl_clean(sdl_base* base);
