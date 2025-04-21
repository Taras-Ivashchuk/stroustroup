#include "game.h"
#include "help.h"
#include <iostream>
#include <sstream>
#include "Random.h"

using namespace std;
 
Help::Input_type which_shooting_range()
{
    string prompt =  "Which shooting range?(1 - 3)\n";
    string pattern = "1-2-3";
    Help::Input_type sr = Help::get_input(prompt, pattern);
    return sr;
}
 
Help::Input_type get_which_room(Player& p) {
    ostringstream prompt; 
    ostringstream pattern;

    prompt << "Which room (";
    const Room* r = p.get_loc();
    const vector<Room*>& exits = r->get_exits(); 

    for (int i = 0; i < exits.size(); ++i) {
        if(exits[i] != nullptr) {
            prompt << exits[i]->get_id();
            pattern << exits[i]->get_id();
        }
        else 
            continue;

        if(i < (exits.size() - 1) && exits.at(i + 1)) { 
            prompt << "/";
            pattern << "-";
        }
    }

    prompt << "?)\n";
    string pro = prompt.str();
    string pat = pattern.str();

    Help::Input_type room_id = Help::get_input(pro, pat);
    return room_id;
}

void Game::init()
{
    for(Bat& b: bats) // accomodate some bats
        c.accom_creature(&b);
    for(Pit& p: pits) // accomodate some pits
        c.accom_creature(&p);
    c.accom_creature(&p); // accomodate player in a room
    c.accom_creature(&w); // accomodate wumpus somewhere
}

bool Game::check_room_connection(Room *r, Room *rr) 
{
    for(const Room* e: r->get_exits())
        if (e->get_id() == rr->get_id())
            return true;

    return false;
}
 
void Game::construct_shoot_path(int shoot_range, vector<Room*>* shoot_path)
{
	vector<int> rids;	     // keep track of duplicates
	rids.push_back( p.get_loc() ->get_id()); // we don't want the player's room
	Room *r = nullptr;      // room in sp
	Room *prev_r = nullptr; // prev room in sp
	for (int i = 1; i <= shoot_range; ++i) {
		ostringstream prompt;
		prompt << "Which room to shoot " << i << "?\n";
		string pro = prompt.str();
        int rid;
		while (true) { // get a room to shoot through
			do
				rid = Help::get_input(pro, "");
			while (rid < Help::R1 || rid > Help::R20);

			auto is_dup = [rid](int id) { return (rid == id); };
			auto dup_found = find_if(rids.begin(), rids.end(), is_dup);
			if (dup_found == rids.end()) {
                rids.push_back(rid);
                break;
            }
            cout << "The room's already chosen. Please enter another room \n";
		}
        prev_r = r;
		r = c.get_room_by_id(rid);
        if(prev_r == nullptr) { // a first room in a row
            shoot_path->push_back(r);
            continue;
        }
        if(check_room_connection(prev_r, r))
            shoot_path->push_back(r);
        else { // there's no path to previous room, so take the random path
               cout << "Some random path taken \n";
               int rn = Random::get(1, prev_r->get_exits().size());
               Room *re = prev_r->get_exit_by_id(rn);
               shoot_path->push_back(re);
        }
	}
}
 
void Game::set_result(Game::Result r)
{ 
    if (r == Game::LOSE || r == Game::WIN) {
        result = r;
        is_game_over = true;
    }
}
 
void Game::good_bye() const
{
    if (result == Game::LOSE)  {
        cout << "YOU LOSE!!!!\n";
    }

    if (result == Game::WIN) {
        cout << "YOU WON!!!!\n";
    }
}
 
void Game::hello() const
{
    cout << "WELCOME TO PLAY HUNT THE WAMPUS!!!\n";

    if (Help::prompt_help() == Help::YES)
        Help::show_help();
}

Creature::Type Game::check_hazard(Room* r)
{
    const vector<Creature*>* creatures = r->get_creatures();

    Creature* bat = r->get_creature(Creature::BAT);
    Creature* wumpus = r->get_creature(Creature::WUMPUS);
    Creature* pit = r->get_creature(Creature::PIT);

    if (pit) 
        return Creature::PIT;

    else if (wumpus) 
        return Creature::WUMPUS;

    else if (bat) 
        return Creature::BAT;

    else 
        return Creature::total; // no hazard!
}
 
void Game::deal_with_shooting()
{
    Help::Input_type sr = which_shooting_range();
    vector<Room*> sp;
    construct_shoot_path(sr, &sp);
    Player::Shot_effect se = p.shoot(sp);
     
    if (se == Player::HIT_WUMPUS) {
        set_result(WIN);
        cout << "HURRAH!!!! YOU KILLED WUMPUS!\n";
        return;
    }
    else if (se == Player::HIT_SELF) {
        set_result(LOSE);
        cout << "YIKES!YOU HIT YOURSELF!!!\n";
        return;
    }
    else if (!p.get_arrows()) {
	    cout << "NO ARROWS!!!\n";
	    set_result(LOSE);
        return;
    }
    else {
        cout << "OOPS! YOU MISSED!!!\n";
        cout << "ARROWS LEFT: " << p.get_arrows() << '\n';
    }

	    // player missed!
	    // wumpus awaken!
	    int rn = Random::get(1, w.get_loc()->get_exits().size());
	    Room *rr = w.get_loc()->get_exit_by_id(rn);
	    w.move(rr);
	    if (w.get_loc() == p.get_loc()) { // wumpus entered player's room!
		    w.eat(&p);
		    set_result(LOSE);
	    }
}

void Game::deal_with_moving() 
{
    Help::Input_type rid = get_which_room(p);
    Room *r = c.get_room_by_id(rid);
    p.move(r);
    while (true) {
        Creature::Type hazard{check_hazard(r)};
        if (hazard == Creature::total) // no hazard, continue play
            return;

        if (hazard == Creature::WUMPUS) { 
            Creature* wumpus = r->get_creature(Creature::WUMPUS);
            if(wumpus == nullptr) 
                exit(1);

            w.eat(&p);
            set_result(LOSE);
            return;
        } else if (hazard == Creature::PIT) {
            Pit* pit = dynamic_cast<Pit*>(r->get_creature(Creature::PIT));
            if(pit == nullptr)
                exit(1);

            pit->swallow(&p);
            set_result(LOSE);
            return;
        } else if (hazard == Creature::BAT) {
            Creature* bat = r->get_creature(Creature::BAT);
            if(bat == nullptr) 
                exit(1);

            Room *rr = c.get_rroom(); // it can be the same room, but we don't care
            p.move(rr);
            bat->move(rr);
            r = rr;
        }
    }
}
 
void Game::play()
{
    hello();
    while (!is_over()) 
        if (get_move_or_shoot() == Help::MOVE)
            deal_with_moving();
        else
            deal_with_shooting();


    good_bye();
}
 
Help::Input_type Game::get_move_or_shoot()
{
    cout << "You entered the room: " << p.get_loc()->get_id() << '\n';
    ostringstream prompt;
    ostringstream pattern;
    prompt << "Move or Shoot(m/s)?\n";
    pattern << "m-s";
    string pro = prompt.str();
    string pat = pattern.str();
    Help::Input_type ms = Help::get_input(pro, pat);
    if(ms == Help::MOVE)
        return Help::MOVE;
    else
        return Help::SHOOT;
}
