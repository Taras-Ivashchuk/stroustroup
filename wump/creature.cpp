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

Player::Shot_effect Player::shoot(vector<Room *> &shoot_path)
{
    arrows -= 1;
    for(const Room* r: shoot_path) {
        if( r->get_id() == loc->get_id())
            return HIT_SELF;
        const vector <Creature*>* creatures = r->get_creatures();

        if(r->get_creature(Creature::WUMPUS) != nullptr)
            return HIT_WUMPUS;
    }
     
    return MISS;
}
 
void Wumpus::move(Room* room) 
{
    Room* croom = get_loc(); // current room
    if (room == croom)      // don't move if the room is current room
        return;

    croom->evict_creature(this);
    set_loc(room);
    room->lodge_creature(this);
}

void Wumpus::eat(Player*)
{
    cout << "WUMPUS: Ha-ha!You're so tasty! I'll eat you up!!\n";
}

void Bat::move(Room* room) 
{
    Room* croom = get_loc(); // current room
    if (room == croom)      // don't move if the room is current room
        return;
    croom->evict_creature(this);
    set_loc(room);
    // we don't lodge the bat, cause the bat has flown away!

    cout << "BAT: I'll snatch you and drop you down to " << room->get_id() <<'\n';
}
 
void Pit::move(Room*) 
{
}

void Pit::swallow(Player*)
{
    cout << "PIT: I've swallowed you, darling!\n";
}
