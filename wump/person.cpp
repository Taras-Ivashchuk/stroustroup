#include "creature.h"
#include "room.h"

void Player::move(Room* room) 
{
    Room* croom = get_loc(); // current room
    if (room == croom)      // don't move if the room is current room
        return;
     
    croom->evict_creature(this);
    set_loc(room);
    room->lodge_creature(this);
}

Player::Shot_effect Player::shoot(vector<Room *> &sp)
{
    Room* ploc = get_loc();
    int pid = ploc->get_id();
    for(const Room* r: sp) {
        if (pid == loc->get_id())
            return HIT_SELF;
        Creature* wump = r->get_creature(Creature::WUMPUS);
        if (wump != nullptr)
            return HIT_WUMPUS;
    }
     
    return MISS;
}
 
void Wumpus::move(Room* room) 
{
    Room* croom = get_loc(); // current room
    if (room == croom)      // don't move if the room is current room
        return;
    set_loc(loc);
}

void Bat::move(Room* room) 
{
    Room* croom = get_loc(); // current room
    if (room == croom)      // don't move if the room is current room
        return;
    croom->evict_creature(Creature::BAT);
    set_loc(room);
}
 
void Pit::move(Room*) 
{
}
