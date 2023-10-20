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

inline int fsg_minmax(int val, int min, int max) {
    int x = val;
    if(x < min) x = min;
    else if(x > max) x = max;
    return x;
}

fsg_map* fsg_map_create(fsg_dungeon* d);
void fsg_map_render(fsg_dungeon* d);
void fsg_map_generate(fsg_dungeon* d);

fsg_entity* fsg_entity_create(fsg_dungeon* d);
void fsg_entity_render(fsg_dungeon* d);

fsg_entity* fsg_player_create(fsg_dungeon* d);
void fsg_player_movement(fsg_dungeon* d);

void fsg_camera_update(fsg_dungeon* d);

#endif
