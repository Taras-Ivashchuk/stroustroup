#ifndef GAME_H
#define GAME_H
 
#include "creature.h"
#include "cave.h"
#include "help.h"
 
class Game {
    public:
	enum Result { NONE, LOSE, WIN };
	Game(int nnrooms = 20, int nnarrows = 5, int nnpits = 3, int nnbats = 3)
        : nrooms{nnrooms}
        , narrows{nnarrows}
        , npits{nnpits}
        , nbats{nnbats}
		, is_game_over{ false }
		, result{ NONE }
        , c{nrooms}
        , p{narrows}
        , pits(npits)
        , bats(nbats)
	{
        init();
    }
    Creature::Type check_hazard(Room *);
    void construct_shoot_path(int, vector<Room*>*);
    void play();

    private:
    int nrooms;
    int narrows;
    int npits;
    int nbats;
    bool is_game_over;
    void set_result(Result r); 
    Result result;
    Cave c;
    Player p;
    Wumpus w;
    vector<Pit> pits;
    vector<Bat> bats;

    void init();
    Help::Input_type get_move_or_shoot();
    bool is_over() const { return is_game_over; }
    void good_bye() const; 
    void hello() const;
    void deal_with_moving();
    void deal_with_shooting();
    bool check_room_connection(Room* r, Room* rr);
};

#endif
