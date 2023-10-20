#include "fsg_define.h"
#include "fsg_types.h"
#include "fsg_util.h"

fsg_dungeon* fsg_dungeon_init(fsg_config* config) {
    fsg_dungeon* d = malloc(sizeof(fsg_dungeon));
    fsg_rnilm(d);

    d->config = config;
    d->map = fsg_map_create(FSG_MAP_WIDTH,
            FSG_MAP_HEIGHT, d->config->max_tiles,
            d->config->resource_path, d->config->sdl_renderer);
    if(!d->map) {
        free(d);
        return NULL;
    }

    d->entity_tail = &d->entity_head;
    d->player = fsg_player_create(d);

    return d;
}

void fsg_movement(fsg_dungeon* dungeon) {
    fsg_player_movement(dungeon);
}

void fsg_update(fsg_dungeon* dungeon) {

}

void fsg_render(fsg_dungeon* dungeon) {
    fsg_map_render(dungeon, dungeon->config->sdl_renderer);
    fsg_entity_render(dungeon, dungeon->config->sdl_renderer);
}
