#ifndef FSG_TYPES_H
#define FSG_TYPES_H

#include "fsg_const.h"
#include "fsg_define.h"
#include <stdint.h>

typedef enum {
    fsg_up,
    fsg_down,
    fsg_left,
    fsg_right,
} fsg_direction;

typedef struct {
    int x, y;
} fsg_vec2;

typedef struct {
    uint16_t screen_width, screen_height;
    uint8_t render_width, render_height;
    uint8_t tilesize;
    uint8_t max_tiles;
    char resource_path[FSG_STR_MAX_LEN];
    char player_path[FSG_STR_MAX_LEN];
    uint8_t player_width, player_height;
    SDL_Renderer** sdl_renderer;
    SDL_Event* sdl_event;
} fsg_config;

typedef struct {
    SDL_Rect* rects;
    int rect_count;
    uint8_t size;
} fsg_minimap;

typedef struct {
    fsg_vec2 size;
    uint8_t data[FSG_MAP_HEIGHT][FSG_MAP_WIDTH];
    SDL_Texture** tiles;
} fsg_map;

typedef struct fsg_entity {
    fsg_vec2 pos;
    fsg_vec2 size;
    fsg_direction facing;
    SDL_Texture* texture;
    struct fsg_entity* next;
} fsg_entity;

typedef struct {
    fsg_config* config;
    fsg_entity entity_head, *entity_tail;
    fsg_map* map;
    fsg_minimap* minimap;
    fsg_entity* player;
    fsg_vec2 camera;
} fsg_dungeon;

#endif
