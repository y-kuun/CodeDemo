#ifndef _ex19_h
#define _ex19_h
#include "object.h"

typedef struct Monster {
    Object proto;
    int hit_points;
} Monster;

int Monster_attack(void *self, int damage);
int Monster_init(void *self);

typedef struct Room {
    Object proto;
    Monster *bad_guy;

    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
} Room;

void *Room_move(void *self, Direction direction);
int Room_attack(void *self, int damage);
int Room_int(void *self);

typedef struct Map {
    Object proto;
    int room_num;
    Room *start;
    Room *location;
    Room **room_list;
} Map;

void *Map_move(void *self, Direction direction);
int Map_attack(void *self, int damage);
int Map_int(void *self);

#endif
