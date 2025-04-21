#include "room.h"
#include <iostream>

using namespace std;

// returns first exit id which is not tied, id 0 to 2
Room** Room::find_unbound_exit() 
{
    for (int i = 0; i < exits.size(); ++i)
        if (exits[i] == nullptr)
            return &(exits[i]);
     
	return nullptr;
}

// try tie the exit id to random room
int Room::bind_exit_to_rr(Room** unb_exit, Room** rr) 
{
    if (unb_exit == nullptr || rr == nullptr)
        return -1;
     
    // check if room has unbound exits
    Room** rr_unb_exit{ (*rr)->find_unbound_exit() };

    if (rr_unb_exit == nullptr) // rroom has no exits to bind
	    return -1;

    *unb_exit = *rr;	    // bind the exit to the rroom
    *rr_unb_exit = this; // bind the rrooms's exit

    return 0;
}

bool Room::is_exit_bound(Room& r) const
{
    for(const Room* e: exits)
        if(e != nullptr && (e->get_id() == r.get_id()))
            return true;
     
    return false;
}

ostream& operator<<(ostream& os, const Room& r)
{
    os << "room (" << r.id << ")\n";
    os << "\troom exits -> ";
    for(Room* exit: r.exits)
        if(exit != nullptr) os << exit->get_id() << " ";
    os << '\n';
    const vector<Creature*>* creatures = r.get_creatures();
    if ((*creatures).size() == 0) {
        return os;
    }
     
    for(const Creature* c: *creatures) {
        if (c != nullptr) {
            switch(c->get_type()) {
                case Creature::PLAYER:
                    cout << "\t has player\n";
                    break;
                case Creature::WUMPUS:
                    cout << "\t has wumpus\n";
                    break;
                case Creature::PIT:
                    cout << "\t has pit\n";
                    break;
                case Creature::BAT:
                    cout << "\t has bat\n";
                    break;
                default:
                    cout << "Unknown creature\n";
                    exit(1);
            }
        }
    }

    return os;
}

void Room::evict_creature(Creature* c) 
{
    if (c == nullptr)
        return;

    auto fn = [c](Creature* cc){ return &(*c) == &(*cc);};
    auto found = find_if(creatures.begin(), creatures.end(), fn);
    if (found == creatures.end())
        return;

    creatures.erase(found);
}

Creature* Room::get_creature(Creature::Type t) const
{
    if (0 > t || t > Creature::Type::total)
        return nullptr;

    auto fn = [t](Creature * c) { return c->get_type() == t; };
    auto found = find_if(creatures.begin(), creatures.end(), fn);
    if (found == creatures.end())
        return nullptr;

    return *found;
}
