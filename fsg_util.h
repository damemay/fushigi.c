#ifndef FSG_UTIL_H
#define FSG_UTIL_H

#include "fsg_define.h"
#include "fsg_types.h"

#define fsg_logv(msg, ...) printf("fushigi.c: " msg "\n", __VA_ARGS__)
#define fsg_log(msg) printf("fushigi.c: " msg "\n")

#define fsg_rnilm(VAL) \
    if(!VAL) { \
        fsg_log("could not allocate memory!"); \
        return NULL; \
    }

#define fsg_min(VAL, MIN) (((VAL) < (MIN)) ? (VAL) : (MIN))
#define fsg_max(VAL, MAX) (((VAL) > (MAX)) ? (VAL) : (MAX))

#define fsg_directed_rect(DIR_, W, H) \
    SDL_Rect dest = { 0, 0, W, H }; \
    switch(DIR_) { \
        case fsg_up: dest.x = 0; break; \
        case fsg_down: dest.x = W; break; \
        case fsg_left: dest.x = 2*W; break;\
        case fsg_right: dest.x = 3*W; break;\
        default: break; \
    }

inline int fsg_minmax(int val, int min, int max) {
    int x = val;
    if(x < min) x = min;
    else if(x > max) x = max;
    return x;
}

fsg_map* fsg_map_create(fsg_dungeon* d);
void fsg_map_render(fsg_dungeon* d);
void fsg_map_generate(fsg_dungeon* d);
void fsg_map_clean(fsg_dungeon* dungeon);

fsg_minimap* fsg_minimap_create(fsg_dungeon* d, uint8_t size);
void fsg_minimap_render(fsg_dungeon* d);
void fsg_minimap_clean(fsg_dungeon* dungeon);

fsg_entity* fsg_entity_create(fsg_dungeon* d);
void fsg_entity_render(fsg_dungeon* d);
void fsg_entity_clean(fsg_dungeon* dungeon);

fsg_entity* fsg_player_create(fsg_dungeon* d);
void fsg_player_movement(fsg_dungeon* d);

void fsg_camera_update(fsg_dungeon* d);

void fsg_dungeon_reload(fsg_dungeon* d);

#endif
