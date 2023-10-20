#ifndef FSG_H
#define FSG_H

#include "fsg_define.h"
#include "fsg_types.h"

fsg_dungeon* fsg_dungeon_init(fsg_config* config);
void fsg_update(fsg_dungeon* dungeon);
void fsg_movement(fsg_dungeon* dungeon);
void fsg_render(fsg_dungeon* dungeon);
void fsg_clean(fsg_dungeon* dungeon);

#endif
