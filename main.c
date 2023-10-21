#include "cdlv/menu/cdlv_menu.h"
#include "fsg_define.h"
#include "fsg.h"
#include "fsg_util.h"

#define TITLE   "fushigi.c"
#define WIDTH   960
#define HEIGHT  544

int main(int argc, char* argv[]) {
    sdl_base* sdl = sdl_create(TITLE, WIDTH, HEIGHT);

    uint64_t tick_cur = 0, tick_last = 0;
    float delta_time = 0;

    fsg_config config = {
        .screen_width   = WIDTH,
        .screen_height  = HEIGHT,
        .render_width   = 30,
        .render_height  = 17,
        .tilesize       = 32,
        .max_tiles      = 3,
        .minimap_tilesize = 4,
        .resource_path  = "res",
        .player_path    = "res/arrow.png",
        .player_width   = 32,
        .player_height  = 64,
        .delta_time     = &delta_time,
        .sdl_renderer   = &sdl->renderer,
        .sdl_event      = &sdl->event,
        .debug          = 0,
    };

    fsg_dungeon* dungeon = fsg_dungeon_init(&config);
    tick_last = SDL_GetTicks64();

    while(sdl->run) {
        tick_cur = SDL_GetTicks64();

        if(tick_last < tick_cur) {
            while(SDL_PollEvent(&sdl->event)) {
                if(sdl->event.type == SDL_QUIT) sdl->run = 0;
                if(dungeon->config->debug 
                    && sdl->event.type == SDL_KEYDOWN 
                    && sdl->event.key.keysym.sym == SDLK_r) {
                    fsg_clean(dungeon);
                    dungeon = fsg_dungeon_init(&config);
                }
                fsg_player_keybinding(dungeon);
            }

            fsg_update(dungeon);

            SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 255);
            SDL_RenderClear(sdl->renderer);
            fsg_render(dungeon);
            SDL_RenderPresent(sdl->renderer);

            delta_time = (tick_cur - tick_last)/1000.0f;
            tick_last = tick_cur;
        }
    }

    fsg_clean(dungeon);
    sdl_clean(sdl);
    return 0;
}
