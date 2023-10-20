#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

static inline SDL_Texture** load_tiles(char* path, uint8_t max_tiles, SDL_Renderer** r) {
    SDL_Texture** ts = malloc(max_tiles*sizeof(SDL_Texture*));
    fsg_rnilm(ts);

    char file[FSG_STR_MAX_LEN];
    for(uint8_t i=1; i<=max_tiles; i++) {
        sprintf(file, "%s/%d.png", path, i);
        ts[i] = IMG_LoadTexture(*r, file);
        fsg_rnilm(ts[i]);
    }

    return ts;
}

fsg_map* fsg_map_create(uint8_t w, uint8_t h, uint8_t mt, char* p, SDL_Renderer** r) {
    fsg_map* m = malloc(sizeof(fsg_map));
    fsg_rnilm(m);

    m->size.x = w, m->size.y = h;
    m->tiles = load_tiles(p, mt, r);
    if(!m->tiles) {
        free(m);
        return NULL;
    }

    // Populate the map
    for(uint8_t y=0; y<h; y++)
        for(uint8_t x=0; x<w; x++)
            m->data[y][x] = FSG_TILE_GROUND;

    return m;
}

static inline void render_tiles(fsg_dungeon* d, SDL_Renderer** r) {
    for(uint8_t y=0; y<d->config->render_height; y++)
        for(uint8_t x=0; x<d->config->render_width; x++) {
            uint8_t mx = d->camera.x + x;
            uint8_t my = d->camera.y + y;
            uint8_t n = d->map->data[my][mx];
            if(n > FSG_TILE_NONE) {
                SDL_Rect rect = { 
                    x*d->config->tilesize, 
                    y*d->config->tilesize,
                    d->config->tilesize,
                    d->config->tilesize
                };
                SDL_RenderCopy(*r, d->map->tiles[n], NULL, &rect);
            }
        }
}

void fsg_map_render(fsg_dungeon* d, SDL_Renderer** r) {
    render_tiles(d, r);
}
