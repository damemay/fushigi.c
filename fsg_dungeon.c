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
}
