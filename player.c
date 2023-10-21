#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

static inline int update_camera(int val, int width, int render_width) {
    int x = val;
    x -= render_width/2;
    x = fsg_minmax(x, 0, width - render_width+1);
    return x;
}

fsg_entity* fsg_player_create(fsg_dungeon* d) {
    fsg_entity* player = fsg_entity_create(d);
    fsg_rnilm(player);
    player->pos.x = 5, player->pos.y = 5;
    player->texture = IMG_LoadTexture(*d->config->sdl_renderer, d->config->player_path);
    player->size.x = d->config->player_width,
    player->size.y = d->config->player_height;
    player->type = fsg_player;
    player->data = malloc(sizeof(uint8_t));
    memset(player->data, 0, sizeof(uint8_t));

    return player;
}

void fsg_camera_update(fsg_dungeon* d) {
    d->camera.x = update_camera(d->player->pos.x, d->map->size.x, d->config->render_width);
    d->camera.y = update_camera(d->player->pos.y, d->map->size.y, d->config->render_height);
}

static inline void player_move(fsg_dungeon* d, int dx, int dy) {
    int x = d->player->pos.x + dx;
    x = fsg_minmax(x, 0, d->map->size.x);
    int y = d->player->pos.y + dy;
    y = fsg_minmax(y, 0, d->map->size.y);

    if((d->map->data[y][x] != FSG_TILE_NONE && d->map->data[y][x] != FSG_TILE_WALL) || d->config->debug) {
        d->player->pos.x = x;
        d->player->pos.y = y;
        d->camera.x = update_camera(x, d->map->size.x, d->config->render_width);
        d->camera.y = update_camera(y, d->map->size.y, d->config->render_height);
        memset(d->player->data, 1, sizeof(uint8_t));
    }

    if(d->map->data[y][x] == FSG_TILE_STAIRS) fsg_dungeon_reload(d);
}

void fsg_player_movement(fsg_dungeon *d) {
    if(*((uint8_t*)d->player->data)) {
        int dx = 0, dy = 0;
        switch(d->player->facing) {
            case fsg_up:    dx = 0,  dy = -1; break;
            case fsg_down:  dx = 0,  dy = 1; break;
            case fsg_left:  dx = -1, dy = 0; break;
            case fsg_right: dx = 1,  dy = 0; break;
            default:break;
        }
        player_move(d, dx, dy);
        SDL_Delay(100);
    }
}

void fsg_player_keybinding(fsg_dungeon* d) {
    if(d->config->sdl_event->type == SDL_KEYDOWN || d->config->sdl_event->type == SDL_CONTROLLERBUTTONDOWN) {
        if(d->config->sdl_event->key.keysym.sym == SDLK_UP
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
            d->player->facing = fsg_up;
            memset(d->player->data, 1, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_DOWN
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
            d->player->facing = fsg_down;
            memset(d->player->data, 1, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_LEFT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
            d->player->facing = fsg_left;
            memset(d->player->data, 1, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_RIGHT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
            d->player->facing = fsg_right;
            memset(d->player->data, 1, sizeof(uint8_t));
        }
    }
    if(d->config->sdl_event->type == SDL_KEYUP || d->config->sdl_event->type == SDL_CONTROLLERBUTTONUP) {
        if(d->config->sdl_event->key.keysym.sym == SDLK_UP
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_UP) {
            memset(d->player->data, 0, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_DOWN
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_DOWN) {
            memset(d->player->data, 0, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_LEFT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_LEFT) {
            memset(d->player->data, 0, sizeof(uint8_t));
        } else if(d->config->sdl_event->key.keysym.sym == SDLK_RIGHT
                || d->config->sdl_event->cbutton.button == SDL_CONTROLLER_BUTTON_DPAD_RIGHT) {
            memset(d->player->data, 0, sizeof(uint8_t));
        }
    }
}
