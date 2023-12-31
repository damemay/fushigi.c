#include "types.h"
#include "util.h"
#include "fsg.h"

static inline int rect_place(fsg_dungeon* d, SDL_Rect* rect) {
    if(rect->x < 1 || rect->y < 1 
            || rect->x + rect->w > FSG_MAP_WIDTH - 1 || rect->y + rect->h > FSG_MAP_HEIGHT - 1)
        return 0;

    for(int y=rect->y; y<rect->y+rect->h; y++)
        for(int x=rect->x; x<rect->x+rect->w; x++)
            if(d->map->data[y][x] > 0) return 0;

    for(int y=rect->y-1; y<rect->y+rect->h+1; y++)
        for(int x=rect->x-1; x<rect->x+rect->w+1; x++) {
            if(x==rect->x-1 || y==rect->y-1 || x==rect->x+rect->w || y==rect->y+rect->h)
                d->map->data[y][x] = d->config->tile_wall;
            else d->map->data[y][x] = d->config->tile_ground;
        }

    return 1;
}

static inline void add_exits(fsg_room* room, SDL_Rect* rect) {
    for(uint8_t i=0; i<fsg_direction_count; i++) {
        switch(i) {
            case fsg_up:
                room->exit[i].x = rect->x;
                room->exit[i].y = rect->y-1;
                break;
            case fsg_down:
                room->exit[i].x = rect->x;
                room->exit[i].y = rect->y+rect->h;
                break;
            case fsg_left:
                room->exit[i].x = rect->x-1;
                room->exit[i].y = rect->y;
                break;
            case fsg_right:
                room->exit[i].x = rect->x+rect->w;
                room->exit[i].y = rect->y;
                break;
            default:break;
        }
    }
}

static inline int room_make(fsg_dungeon* d, int x, int y, fsg_direction dir) {
    const int min_size = 3, max_size = 10;
    SDL_Rect room;
    room.w = min_size + rand() % (max_size-min_size+1);
    room.h = min_size + rand() % (max_size-min_size+1);

    switch(dir) {
        case fsg_up:
            room.x = x - room.w / 2;
            room.y = y - room.h;
            break;
        case fsg_down:
            room.x = x - room.w / 2;
            room.y = y + 1;
            break;
        case fsg_left:
            room.x = x - room.w;
            room.y = y - room.h / 2;
            break;
        case fsg_right:
            room.x = x + 1;
            room.y = y - room.h / 2;
            break;
        default: break;
    }

    if(!rect_place(d, &room)) return 0;

    uint8_t i = d->map->room_count;
    d->map->rooms[i].rect.x = room.x;
    d->map->rooms[i].rect.y = room.y;
    d->map->rooms[i].rect.w = room.w;
    d->map->rooms[i].rect.h = room.h;
    add_exits(&d->map->rooms[i], &room);
    d->map->room_count++;
    return 1;
}

static inline void place_elements(fsg_dungeon* d, size_t tries, int max) {
    uint8_t room_chance = 50;
    for(size_t i=0; i<tries; i++) {
        uint8_t room = rand()%d->map->room_count;
        uint8_t wall = rand()%fsg_direction_count;
        if(rand()%100 < room_chance) {
            int x = d->map->rooms[room].exit[wall].x;
            int y = d->map->rooms[room].exit[wall].y;
            if(d->map->room_count < max && room_make(d, x, y, wall))
                d->map->data[y][x] = d->config->tile_ground;
        }
    }
}

static inline fsg_room* init_rooms(int max) {
    fsg_room* rooms = calloc(max, sizeof(fsg_room));
    fsg_rnilm(rooms);
    return rooms;
}

static inline void place_object(fsg_dungeon* d, int *dx, int *dy, int tile) {
    while(d->map->data[*dy][*dx] != d->config->tile_ground) {
        int room = rand()%d->map->room_count;
        fsg_room* r = &d->map->rooms[room];
        int x = r->rect.x+1 + rand()%(r->rect.x + r->rect.w-2);
        *dx = fsg_minmax(x, 0, FSG_MAP_WIDTH);
        int y = r->rect.y+1 + rand()%(r->rect.y + r->rect.h-2);
        *dy = fsg_minmax(y, 0, FSG_MAP_HEIGHT);
    }
    if(tile) {
        d->map->data[*dy][*dx] = tile;
    }
}

static inline void place_tile(fsg_dungeon* d, int tile) {
    int dx = 0, dy = 0;
    place_object(d, &dx, &dy, tile);
}

static inline void place_player(fsg_dungeon* d) {
    place_object(d, &d->player->pos.x, &d->player->pos.y, 0);
}

int fsg_find_room_player_is_in(fsg_dungeon* d) {
    int room = -1;
    for(int i=0; i<d->map->room_count; i++) {
        fsg_room* r = &d->map->rooms[i];
        if(fsg_is_between(d->player->pos.x, r->rect.x, r->rect.x+r->rect.w) &&
                fsg_is_between(d->player->pos.y, r->rect.y, r->rect.y+r->rect.h))
            room = i;
    }
    return room;
}

void fsg_map_generate(fsg_dungeon* d) {
    for(uint8_t y=0; y<FSG_MAP_HEIGHT; y++)
        for(uint8_t x=0; x<FSG_MAP_WIDTH; x++)
            d->map->data[y][x] = d->config->tile_none;

    int max_rooms = FSG_MAP_HEIGHT*FSG_MAP_WIDTH/100;
    d->map->rooms = init_rooms(max_rooms);
    d->map->room_count = 0;

    if(!room_make(d, FSG_MAP_WIDTH/2, FSG_MAP_HEIGHT/2, rand()%fsg_direction_count)) 
        return;
    
    place_elements(d, 5000, max_rooms);
    place_player(d);
    place_tile(d, d->config->tile_stairs);

}
