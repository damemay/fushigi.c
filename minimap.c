#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

static inline int map_ground_count(fsg_dungeon* d) {
    int count = 0;
    for(uint8_t y=0; y<FSG_MAP_HEIGHT; y++)
        for(uint8_t x=0; x<FSG_MAP_WIDTH; x++)
            if(d->map->data[y][x] == FSG_TILE_GROUND) count++;
    return count;
}

static inline SDL_Rect* minimap_rects_malloc(int count) {
    SDL_Rect* rects = calloc(count, sizeof(SDL_Rect));
    fsg_rnilm(rects);
    return rects;
}

void fsg_add_to_minimap(fsg_dungeon* d, int x, int y) {
    int i_ = d->minimap->iter;
    for(int i=0; i<i_; i++) {
        SDL_Rect* r = &d->minimap->rects[i];
        if(r->x == x*d->minimap->size && r->y == y*d->minimap->size) return;
    }

    if(i_<d->minimap->rect_count && d->map->data[y][x] != FSG_TILE_WALL && d->map->data[y][x] != FSG_TILE_NONE) {
       d->minimap->rects[i_].x = x*d->minimap->size;
       d->minimap->rects[i_].y = y*d->minimap->size;
       d->minimap->rects[i_].w = d->minimap->size;
       d->minimap->rects[i_].h = d->minimap->size;
       d->minimap->iter++;
    }
}

static inline void minimap_rects_make(fsg_dungeon* d, SDL_Rect* rects, uint8_t size, int count) {
    int i=0;
    for(uint8_t y=0; y<FSG_MAP_HEIGHT; y++)
        for(uint8_t x=0; x<FSG_MAP_WIDTH; x++) {
            if(i<count &&
                    d->map->data[y][x] != FSG_TILE_NONE && 
                    d->map->data[y][x] != FSG_TILE_WALL) {
                rects[i].x = x*size;
                rects[i].y = y*size;
                rects[i].w = size;
                rects[i].h = size;
                i++;
            }
        }
}

fsg_minimap* fsg_minimap_create(fsg_dungeon* d, uint8_t size) {
    fsg_minimap* m = malloc(sizeof(fsg_minimap));
    fsg_rnilm(m);

    m->size = size;
    m->iter = 0;
    m->rect_count = map_ground_count(d);
    m->rects = minimap_rects_malloc(m->rect_count);
    if(!m->rects) {
        free(m);
        return NULL;
    }
    // minimap_rects_make(d, m->rects, m->size, m->rect_count);

    return m;
}

void fsg_minimap_render(fsg_dungeon* d) {
    SDL_Rect rect = {
        d->player->pos.x*d->minimap->size,
        d->player->pos.y*d->minimap->size,
        d->minimap->size,
        d->minimap->size
    };
    SDL_SetRenderDrawBlendMode(*d->config->sdl_renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(*d->config->sdl_renderer, 0, 0, 255, 128);
    SDL_RenderFillRects(*d->config->sdl_renderer, d->minimap->rects, d->minimap->rect_count);
    SDL_SetRenderDrawColor(*d->config->sdl_renderer, 255, 255, 0, 255);
    SDL_RenderFillRect(*d->config->sdl_renderer, &rect);
}
