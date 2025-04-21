// #include "room.h"
// #include <sstream>
// #include <iostream>
#include "game.h"

// void print_creature(const Creature* c) {
//     switch(c->get_type()) {
//         case Creature::PLAYER:
//             cout << "Creature: Player\n";
//             break;
//         case Creature::WUMPUS:
//             cout << "Creature: Wumpus\n";
//             break;
//         case Creature::BAT:
//             cout << "Creature: Bat\n";
//             break;
//         case Creature::PIT:
//             cout << "Creature: PIT\n";
//             break;
//         case Creature::total:
//             cout << "Creature: total\n";
//             break;
//         default:
//             cout << "Unknown creature\n";
//             break;
//     }
// }
//
 
int main()
{
    while (true) {
	    Game g;
	    g.play();
    }
}
 
