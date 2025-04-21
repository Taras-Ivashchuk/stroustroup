#include "cave.h"
#include "Random.h"
#include "creature.h"
#include <iostream>
#include "room.h"

using namespace std;

Room* Cave::get_room_by_id(int id)
{
	if (id < 1 || id > rooms.size())
		return nullptr;
	return &(rooms.at(id - 1));
}

Cave::Cave(int nrooms)
{ 
    assert(nrooms > 1 && "Number of rooms must be greater than 1\n");
    for(int i = 1; i <= nrooms; ++i) {
        Room room{i};
        rooms.push_back(room);
    }
    init_rooms();
}

void Cave::accom_creature(Creature* c)	
{
    if(!c)
        return;
     
    Room* rr = get_rroom();
    while(has_hazard(rr))
        rr = get_rroom();

    c->set_loc(rr);
    rr->lodge_creature(c);
}

Room* Cave::get_rroom()
{
    int rinx = Random::get(0, rooms.size() - 1);
    return &(rooms.at(rinx));
}

bool Cave::has_hazard(const Room* r) const
{
    return (r->get_creatures()->size() > 0);
}

void Cave::print() const
{
    for(const Room& room: rooms)
        cout << room;
}


// make all room's exits tied 
void Cave::init_room_exits(Room& room)
{
    int max_tries = 100;
    Room** unb_exit = room.find_unbound_exit();
    while (unb_exit != nullptr && max_tries--) {
        Room* rr = get_rroom();
        while ((rr->get_id() == room.get_id()) || room.is_exit_bound(*rr))
            rr = get_rroom();

        room.bind_exit_to_rr(unb_exit, &rr); // try to tie room 
        unb_exit = room. find_unbound_exit();
    }
}


void Cave::init_rooms()
{
    for (Room& room: rooms) 
        init_room_exits(room);
}
