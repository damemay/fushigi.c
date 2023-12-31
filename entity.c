#include "types.h"
#include "util.h"

fsg_entity* fsg_entity_create(fsg_dungeon* d) {
    fsg_entity* e;
    e = malloc(sizeof(fsg_entity));
    fsg_rnilm(e);
    
    e->pos.x = 0, e->pos.y = 0;
    e->facing = fsg_right;
    e->texture = NULL;
    e->size.x = 0, e->size.y = 0;
    e->next = NULL;

    d->entity_tail->next = e;
    d->entity_tail = e;

    return e;
}

void fsg_entity_render(fsg_dungeon* d) {
    for(fsg_entity* e = d->entity_head.next; e != NULL; e = e->next) {
        int x = ((e->pos.x - d->camera.x) * d->config->tilesize) + (d->config->tilesize/2);
        int y = ((e->pos.y - d->camera.y) * d->config->tilesize) + (d->config->tilesize/2);

        fsg_directed_rect(e->facing, e->size.x, e->size.y);
        SDL_Rect rect = {
            x - e->size.x/2,
            y - e->size.y+d->config->tilesize/2,
            e->size.x,
            e->size.y
        };
        SDL_RenderCopy(*d->config->sdl_renderer, e->texture, &dest, &rect);
    }
}
