#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"
#include "fsg.h"

static inline int rect_place(fsg_dungeon* d, SDL_Rect* rect, uint8_t tile) {
    if(rect->x < 1 || rect->y < 1 
            || rect->x + rect->w > FSG_MAP_WIDTH - 1 || rect->y + rect->h > FSG_MAP_HEIGHT - 1)
        return 0;

    for(int y=rect->y; y<rect->y+rect->h; ++y)
        for(int x=rect->x; x<rect->x+rect->w; ++x)
            if(d->map->data[y][x] > 0) return 0;

    for(int y=rect->y; y<rect->y+rect->h; ++y)
        for(int x=rect->x; x<rect->x+rect->h; ++x) {
            d->map->data[y][x] = tile;
        }

    return 1;
}

static inline int room_make(fsg_dungeon* d, int x, int y, fsg_direction dir) {
    const int min_size = 3, max_size = 10;
    SDL_Rect room;
    room.w = min_size + rand() % (max_size-min_size+1);
    room.h = min_size + rand() % (max_size-min_size+1);

    switch(dir) {
        case fsg_up:
            room.x = x - room.w / 2;
            room.y = y - room.h;
            break;
        case fsg_down:
            room.x = x - room.w / 2;
            room.y = y + 1;
            break;
        case fsg_left:
            room.x = x - room.w;
            room.y = y - room.h / 2;
            break;
        case fsg_right:
            room.x = x + 1;
            room.y = y - room.h / 2;
            break;
        default: break;
    }

    if(rect_place(d, &room, FSG_TILE_GROUND)) return 1;
    return 0;
}

static inline void print(fsg_dungeon* dungeon) {
    for(uint8_t y=0; y<FSG_MAP_HEIGHT; ++y) {
        for(uint8_t x=0; x<FSG_MAP_WIDTH; ++x) {
            switch(dungeon->map->data[y][x]) {
                case FSG_TILE_NONE: putchar(' '); break;
                case FSG_TILE_GROUND: putchar('.'); break;
                case FSG_TILE_WALL: putchar('#'); break;
                case FSG_TILE_STAIRS: putchar('>'); break;
            }
        }
        putchar('\n');
    }
}

static inline int try_element(fsg_dungeon* d) {
    for(int i=0; i<1000; ++i) {
        room_make(d, rand()%FSG_MAP_WIDTH, rand()%FSG_MAP_HEIGHT, rand()%4);
    }
    return 0;
}

static inline void make_walls(fsg_dungeon* d) {
    for(int y=0; y<FSG_MAP_HEIGHT; y++)
        for(int x=0; x<FSG_MAP_WIDTH; x++) {
            if(d->map->data[y][x] == FSG_TILE_NONE) {
                if(d->map->data[y+1][x] == FSG_TILE_GROUND)      d->map->data[y][x] = FSG_TILE_WALL;
                else if(d->map->data[y-1][x] == FSG_TILE_GROUND) d->map->data[y][x] = FSG_TILE_WALL;
                else if(d->map->data[y][x+1] == FSG_TILE_GROUND) d->map->data[y][x] = FSG_TILE_WALL;
                else if(d->map->data[y][x-1] == FSG_TILE_GROUND) d->map->data[y][x] = FSG_TILE_WALL;
            } else { 
                if(y == 0 || x == 0 || x == FSG_MAP_WIDTH || y == FSG_MAP_HEIGHT)
                    d->map->data[y][x] = FSG_TILE_WALL;
            }
        }
}

void fsg_map_generate(fsg_dungeon* d) {
    for(uint8_t y=0; y<FSG_MAP_HEIGHT; ++y)
        for(uint8_t x=0; x<FSG_MAP_WIDTH; ++x)
            d->map->data[y][x] = FSG_TILE_NONE;

    if(!room_make(d, FSG_MAP_WIDTH/2, FSG_MAP_HEIGHT/2, rand()%4)) {
        puts("cannot make first room");
        return;
    }

    for(int i = 1; i<10; ++i) try_element(d);
    make_walls(d);
    print(d);
}
