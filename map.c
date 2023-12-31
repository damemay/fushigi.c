#include "types.h"
#include "util.h"

static inline SDL_Texture** load_tiles(char* path, uint8_t max_tiles, SDL_Renderer** r) {
    SDL_Texture** ts = malloc((max_tiles+1)*sizeof(SDL_Texture*));
    fsg_rnilm(ts);

    char file[1024];
    for(uint8_t i=1; i<=max_tiles; i++) {
        sprintf(file, "%s/%d.png", path, i);
        ts[i] = IMG_LoadTexture(*r, file);
        fsg_rnilm(ts[i]);
    }

    return ts;
}

fsg_map* fsg_map_create(fsg_dungeon* d) {
    fsg_map* m = malloc(sizeof(fsg_map));
    fsg_rnilm(m);

    m->size.x = FSG_MAP_WIDTH, m->size.y = FSG_MAP_HEIGHT;
    m->tiles = load_tiles(d->config->resource_path, d->config->max_tiles, d->config->sdl_renderer);
    if(!m->tiles) {
        free(m);
        return NULL;
    }
    m->rooms = NULL;

    return m;
}

int fsg_is_between(int a, int b, int c) {
    if(a <= c && a >= b) return 1;
    return 0;
}

static inline void render_tiles(fsg_dungeon* d, SDL_Renderer** r) {
    for(uint8_t y=0; y<d->config->render_height; y++)
        for(uint8_t x=0; x<d->config->render_width; x++) {
            uint8_t mx = d->camera.x + x;
            mx = fsg_minmax(mx, 0, FSG_MAP_WIDTH-1);
            uint8_t my = d->camera.y + y;
            my = fsg_minmax(my, 0, FSG_MAP_HEIGHT-1);
            uint8_t n = d->map->data[my][mx];
            if(n > d->config->tile_none && n <= d->config->max_tiles) {
                int room = fsg_find_room_player_is_in(d);
                fsg_room* room_ = &d->map->rooms[room];
                if(room >= 0 &&
                        fsg_is_between(mx, room_->rect.x-1, room_->rect.x+room_->rect.w+1) &&
                        fsg_is_between(my, room_->rect.y-1, room_->rect.y+room_->rect.h+1)) {
                    fsg_add_to_minimap(d, mx, my);
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
}

void fsg_map_render(fsg_dungeon* d) {
    render_tiles(d, d->config->sdl_renderer);
}
