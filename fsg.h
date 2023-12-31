#ifndef FSG_H
#define FSG_H

#include "types.h"

fsg_dungeon* fsg_dungeon_init(fsg_config* config);
void fsg_update(fsg_dungeon* dungeon);
void fsg_player_keybinding(fsg_dungeon* d);
void fsg_render(fsg_dungeon* dungeon);
void fsg_clean(fsg_dungeon* dungeon);

#endif
