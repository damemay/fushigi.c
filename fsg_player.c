#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

static inline int minmax(int val, int min, int max) {
    int x = val;
    if(x < min) x = 0;
    else if(x > max) x = max;
    return x;
}

static inline int update_camera(int val, int width, int render_width) {
    int x = val;
    x -= render_width/2;
    x = minmax(x, 0, width - render_width);
    return x;
}


fsg_entity* fsg_player_create(fsg_dungeon* d) {
    fsg_entity* player = fsg_entity_create(d);
    fsg_rnilm(player);
    player->pos.x = 5, player->pos.y = 5;
    player->texture = IMG_LoadTexture(*d->config->sdl_renderer, d->config->player_path);
    player->size.x = d->config->player_width,
    player->size.y = d->config->player_height;
    d->camera.x = update_camera(player->pos.x, d->map->size.x, d->config->render_width);
    d->camera.y = update_camera(player->pos.y, d->map->size.y, d->config->render_height);

    return player;
}

static inline void player_move(fsg_dungeon* d, int dx, int dy) {
    int x = d->player->pos.x + dx;
    x = minmax(x, 0, d->map->size.x);
    int y = d->player->pos.y + dy;
    y = minmax(y, 0, d->map->size.y);

    d->player->pos.x = x;
    d->player->pos.y = y;
    d->camera.x = update_camera(x, d->map->size.x, d->config->render_width);
    d->camera.y = update_camera(y, d->map->size.y, d->config->render_height);

    printf("%d %d  %d %d\n", d->player->pos.x, d->player->pos.y, d->camera.x, d->camera.y);
}

void fsg_player_movement(fsg_dungeon* d) {
    if(d->config->sdl_event->type == SDL_KEYDOWN || d->config->sdl_event->type == SDL_CONTROLLERBUTTONDOWN) {
        if(d->config->sdl_event->key.keysym.sym == SDLK_UP
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
            player_move(d, 0, -1);
            d->player->facing = fsg_up;
        }
        if(d->config->sdl_event->key.keysym.sym == SDLK_DOWN
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
            player_move(d, 0, 1);
            d->player->facing = fsg_down;
        }
        if(d->config->sdl_event->key.keysym.sym == SDLK_LEFT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
            player_move(d, -1, 0);
            d->player->facing = fsg_up;
        }
        if(d->config->sdl_event->key.keysym.sym == SDLK_RIGHT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
            player_move(d, 1, 0);
            d->player->facing = fsg_right;
        }
    }
}
