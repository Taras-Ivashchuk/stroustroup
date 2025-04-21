#ifndef ROOM_H
#define ROOM_H
 
#include <cassert>
#include <vector>
#include <iostream>
#include "creature.h"

using namespace std;
 
class Room {
    public:

        Room(int i = 1, int nexits = 3)
            : id{ i }
            , exits(nexits)
        {
            assert(id >= 1 && "Room's id must be larger than 0");
            assert(exits.size() > 1 &&
                   "Number of exits for each room must be at least 1");

        }
             
	    Room** find_unbound_exit();
        int get_id() const { return id; }
        Room* get_exit_by_id(int id) const { return exits.at(id - 1); }
        int bind_exit_to_rr(Room**, Room**); // bind exit with random room
        bool is_exit_bound(Room&) const;
        const vector<Room*>& get_exits() const { return exits; }
        friend ostream& operator<<(ostream& os, const Room& room);
        void lodge_creature(Creature* c) { creatures.push_back(c); }
        void evict_creature(Creature* c); 
        Creature* get_creature(Creature::Type) const;
        const vector <Creature*>* get_creatures() const { return &creatures; }
    private:
        vector <Creature*> creatures; 
        int id;
        vector<Room *> exits;
};
 

#endif
