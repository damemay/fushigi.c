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
    fsg_direction_count,
} fsg_direction;

typedef enum {
    fsg_none,
    fsg_player,
    fsg_enemy,
} fsg_entity_t;

typedef struct {
    uint16_t screen_width, screen_height;
    uint8_t render_width, render_height;
    uint8_t tilesize;
    uint8_t max_tiles;
    uint8_t minimap_tilesize;
    char resource_path[FSG_STR_MAX_LEN];
    char player_path[FSG_STR_MAX_LEN];
    uint8_t player_width, player_height;
    float* delta_time;
    int debug;
    SDL_Renderer** sdl_renderer;
    SDL_Event* sdl_event;
} fsg_config;

typedef struct {
    SDL_Rect* rects;
    int rect_count;
    int iter;
    uint8_t size;
} fsg_minimap;

typedef struct {
    SDL_Rect rect;
    SDL_Point exit[4];
} fsg_room;

typedef struct {
    SDL_Point size;
    uint8_t data[FSG_MAP_HEIGHT][FSG_MAP_WIDTH];
    fsg_room* rooms;
    uint8_t room_count;
    SDL_Texture** tiles;
} fsg_map;

typedef struct fsg_entity {
    SDL_Point pos;
    SDL_Point size;
    fsg_direction facing;
    fsg_entity_t type;
    void (*data);
    SDL_Texture* texture;
    struct fsg_entity* next;
} fsg_entity;

typedef struct {
    fsg_config* config;
    fsg_entity entity_head, *entity_tail;
    fsg_map* map;
    fsg_minimap* minimap;
    fsg_entity* player;
    SDL_Point camera;
} fsg_dungeon;

#endif
