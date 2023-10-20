#include "cdlv/menu/cdlv_menu.h"
#include "fsg_define.h"
#include "fsg.h"

#define TITLE   "fushigi.c"
#define WIDTH   960
#define HEIGHT  544

int main(int argc, char* argv[]) {
    sdl_base* sdl = sdl_create(TITLE, WIDTH, HEIGHT);

    fsg_config config = {
        .screen_width   = WIDTH,
        .screen_height  = HEIGHT,
        .render_width   = 30,
        .render_height  = 17,
        .tilesize       = 32,
        .max_tiles      = 2,
        .resource_path  = "res",
        .player_path    = "res/char.png",
        .player_width   = 32,
        .player_height  = 64,
        .sdl_renderer   = &sdl->renderer,
        .sdl_event      = &sdl->event,
    };

    fsg_dungeon* dungeon = fsg_dungeon_init(&config);

    while(sdl->run) {
        while(SDL_PollEvent(&sdl->event)) {
            if(sdl->event.type == SDL_QUIT) sdl->run = 0;
            if(sdl->event.type == SDL_KEYUP || sdl->event.type == SDL_CONTROLLERBUTTONUP) if(sdl->event.key.keysym.sym == SDLK_r || sdl->event.cbutton.button == SDL_CONTROLLER_BUTTON_RIGHTSHOULDER) {
                fsg_clean(dungeon);
                dungeon = fsg_dungeon_init(&config);
            }
            fsg_movement(dungeon);
        }
        SDL_RenderClear(sdl->renderer);
        fsg_update(dungeon);
        fsg_render(dungeon);
        SDL_RenderPresent(sdl->renderer);
    }

    fsg_clean(dungeon);
    sdl_clean(sdl);
    return 0;
}
