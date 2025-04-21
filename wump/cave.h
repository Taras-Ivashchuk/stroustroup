#ifndef CAVE_H
#define CAVE_H

#include "room.h"
#include <vector>
#include <cassert>

class Creature;
 
class Cave {
    public:
        explicit Cave(int nrooms = 20);
        void print() const;
        void accom_creature(Creature*);	// accomodate character in rooms
        Room* get_rroom(); // get random room
        Room* get_room_by_id(int rid); 
    private:
        vector<Room> rooms;
        // vector<Room*> batsloc; // bats locations
        // vector<Room*> pitsloc; // pits locations
        // Room* ploc; // Player location
        // Room* wloc; // Wumpus location

        void init_room_exits(Room &);	// ties all exits in the room
        // void init_pits(int npits);	        	// allocates pits in rooms
        bool has_hazard(const Room *r) const; // check if room has a creature
        void init_rooms();		        // invokes room initializing
};
 
#endif 
