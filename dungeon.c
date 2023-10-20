#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"
#include <time.h>

fsg_dungeon* fsg_dungeon_init(fsg_config* config) {
    fsg_dungeon* d = malloc(sizeof(fsg_dungeon));
    fsg_rnilm(d);

    d->config = config;

    d->map = fsg_map_create(d);
    if(!d->map) {
        free(d);
        return NULL;
    }

    d->entity_tail = &d->entity_head;
    d->player = fsg_player_create(d);

    srand(time(NULL));

    fsg_map_generate(d);
    d->minimap = fsg_minimap_create(d, 2);
    fsg_camera_update(d);

    return d;
}

void fsg_movement(fsg_dungeon* dungeon) {
    fsg_player_movement(dungeon);
}

void fsg_update(fsg_dungeon* dungeon) {

}

void fsg_render(fsg_dungeon* dungeon) {
    fsg_map_render(dungeon);
    fsg_entity_render(dungeon);
    fsg_minimap_render(dungeon);
}

void fsg_clean(fsg_dungeon* dungeon) {
    for(uint8_t i=0; i<dungeon->config->max_tiles; i++)
        if(dungeon->map->tiles[i]) SDL_DestroyTexture(dungeon->map->tiles[i]);
    if(dungeon->map->tiles) free(dungeon->map->tiles);
    if(dungeon->map) free(dungeon->map);

    if(dungeon->minimap->rects) free(dungeon->minimap->rects);
    if(dungeon->minimap) free(dungeon->minimap);

    for(fsg_entity* e = dungeon->entity_head.next; e != NULL; e = e->next) {
        if(e->texture) SDL_DestroyTexture(e->texture);
        if(e) free(e);
    }

    if(dungeon) free(dungeon);
}
