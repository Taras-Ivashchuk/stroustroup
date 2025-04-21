#ifndef CREATURE_H
#define CREATURE_H
 
#include <vector>

using namespace std;
 
class Room;

class Creature {
    public:
    enum Type{
        PLAYER,
        WUMPUS,
        PIT,
        BAT,
        total
    };
     
	Creature(Type t)
		: loc{ nullptr }
		, type{ t }
	{}
	Room *get_loc() const
	{
		return loc;
	}
	void set_loc(Room *r)
	{
		loc = r;
	}
    
    const Type get_type() const { return type; }

	void virtual move(Room* r) = 0;
	virtual ~Creature()
	{}

    protected:
	Type type;
	Room *loc;
};

class Player : public Creature {
    public:
    enum Shot_effect {
        HIT_SELF,
        HIT_WUMPUS,
        MISS
    };
	explicit Player(int a)
		: Creature{Creature::PLAYER}
		, arrows{ a }
	{}
	void move(Room* r) override;
	Shot_effect shoot(vector<Room *>&); 
    int get_arrows() const { return arrows; }

    private:
	int arrows;
};

class Wumpus : public Creature {
    public:
	Wumpus()
		: Creature{Creature::WUMPUS}
	{}
	void move(Room* r) override;
    void eat(Player*);
};

class Bat : public Creature {
    public:
	Bat()
		: Creature{Creature::BAT}
	{}
	void move(Room* r) override;
};
 
class Pit : public Creature {
    public:
	Pit()
		: Creature{Creature::PIT}
	{}
	void move(Room* r) override;
    void swallow(Player*);
};

#endif
