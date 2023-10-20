#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

static inline void random_walk(fsg_dungeon* d, fsg_map* m) {
    float coverage = 30 + rand()%21;
    uint32_t n = (FSG_MAP_WIDTH * FSG_MAP_HEIGHT) * (coverage*0.01);

    for(uint8_t y=0; y<FSG_MAP_HEIGHT; y++)
        for(uint8_t x=0; x<FSG_MAP_WIDTH; x++)
            m->data[y][x] = FSG_TILE_WALL;

    int x = 1 + rand() % (FSG_MAP_WIDTH);
    int y = 1 + rand() % (FSG_MAP_HEIGHT);
    int dx = 0, dy = 0, straight = 0;
    m->data[y][x] = FSG_TILE_GROUND;

    while(n > 0) {
        straight = fsg_max(straight-1, 0);
        if(straight==0) {
            switch(rand()%5) {
                case 0: dx = 0; dy = -1; break;
                case 1: dx = 0; dy = 1; break;
                case 2: dx = -1; dy = 0; break;
                case 3: dx = 1; dy = 0; break;
                default: straight = 4 + rand()%8; break;
            }
        }

        x = fsg_min(fsg_max(x+dx, 1), FSG_MAP_WIDTH-2);
        y = fsg_min(fsg_max(y+dy, 1), FSG_MAP_HEIGHT-2);
        

        if(m->data[y][x] == FSG_TILE_WALL) {
            m->data[y][x] = FSG_TILE_GROUND;
            n--;
        }
    }

    d->player->pos.x = x;
    d->player->pos.y = y;
}

static inline int count_walls(fsg_map* m, int mx, int my) {
    int n = 0;
    for(int y=-1; y<=1; y++)
        for(int x=-1; x<=1; x++) {
            if((x != 0 || y != 0) && m->data[my+y][mx+x] == FSG_TILE_WALL)
                n++;
        }
    return n;
}

static inline void tidy_walls(fsg_map* m) {
    int walls_removed;
    do {
        walls_removed = 0;
        for(uint8_t y=1; y<FSG_MAP_HEIGHT-1; y++)
            for(uint8_t x=1; x<FSG_MAP_WIDTH-1; x++) {
                if(m->data[y][x] == FSG_TILE_WALL && count_walls(m, x, y) < 2) {
                    walls_removed = 1;
                    m->data[y][x] = FSG_TILE_GROUND;
                }
            }
    } while(walls_removed != 0);
}

void fsg_map_generate(fsg_dungeon* d) {
    random_walk(d, d->map);
    tidy_walls(d->map);
}
