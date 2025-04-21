#include "help.h"
#include <iostream>
#include <limits>
#include <string.h>
#include <vector>
#include <sstream>

namespace Help {
    using namespace std;
     
    Input_type prompt_help() 
    {
        string text = "SHOW INSTRUCTIONS (y/n)?\n";
        string pattern = "y-n";
        Input_type input = Help::get_input(text, pattern);
        return input;
    }

    void show_help()
    {

        string text  = "Wampus lives in a cave of 20 rooms. Each room has 3 tunnels leading to other rooms.\n"
            "In a room there may be a giant bat or a pit. " "If you get into a room with giant bat, \n"
            "the giant bat grabs you into other random room (which may be troublesome). " "If there's a pit\n"
            "in a room, you  fall down the pit and loose the game! " "Wumpus doesn't care the bats, cause he's\n"
            "too big for it and has sucker feet preventing him to fall through the pit. " "Usually Wumpus is \n"
            "asleep. The only thing that wakes him up is shooting. " "After the shooting made he moves to the \n" 
            "other room. " "If he is where you are he eats you up and you loose! " "You can move one room \n" 
            "through the tunnel or shoot Wumpus to kill him. " "You have 5 arrows. " "You loose if you run out \n"
            "of arrows! " "You can shoot arrow going through 1 room up to 3 rooms. " "You shoot by telling which \n"
            "way your arrow goes through. " "If there's no way to next room the arrow chooses the next room by \n"
            "random. " "So called Crooked Arrow. " "If arrow hits Wampus, you win, and if arrow hits you, you lose! \n"
            "You can quit the game anytime you want by pressing 'q' or 'Q'.\n\n";



        cout << text;

    }

    bool has_unextracted_input() 
    {
        return !cin.eof() && cin.peek() != '\n';
    }

    void ignore_line()
    {

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    bool is_token_valid(int t) 
    {
        switch(t) {
            case R1: case R2:
            case R3: case R4:
            case R5: case R6:
            case R7: case R8:
            case R9: case R10:
            case R11: case R12:
            case R13: case R14:
            case R15: case R16:
            case R17: case R18:
            case R19: case R20:
            case YES: case NO:
            case MOVE: case SHOOT:
            case QUIT:
                return true;
            default:
                return false;
        }
    }
    
    int get_token(const string& prompt)
    {
	    int i; // try to read integer at first
	    do {
            cout << prompt;
		    cin >> i;
		    if (cin.eof())      // user pressed ctrl-d
			    exit(0);
		    else if (!cin) {    // ingteger extraction failed
			    cin.clear();         
                char c;         //  then try to read char
                cin >> c;   

                if(has_unextracted_input()) {
                    cin.clear();
                    ignore_line(); // remove bad input
                    continue;
                }

                i = tolower(c);
			    break;
		    } else if (has_unextracted_input()) {
                cin.clear();
			    ignore_line();   // remove bad input
			    continue;
		    }

            if(is_token_valid(i))
                break;

	    } while (true);

	    if (i == QUIT)
		    exit(0);
         
	    return i;
    }

    vector<int> parse_pattern(const string &pattern, const string &delim)
    {
        vector<int> pptokens; // parsed pattern tokens
        char* saveptr = const_cast<char *>(pattern.c_str());
        const char* ptoken;  // pattern token

        while ((ptoken = strtok_r(saveptr, delim.c_str(), &saveptr))) {
            stringstream ss{};
            int i{};
            char c{};

            ss << string(ptoken);
            ss >> i; // first read token as integer
            if (ss.fail()) {
                ss.clear();
                ss >> c; // try to read as char
                 
                if (ss.peek() != EOF)   
                    exit(0);
                 
                i = tolower(c);// add lowercase char

                if (!is_token_valid(i))
                    exit(0);

                pptokens.push_back(i); 
            } else {
                if(!is_token_valid(i))
                    exit(0);
                pptokens.push_back(i); 
            }
        }

        return pptokens;
	}

    bool is_token_match(int token, vector<int> tokens) 
    {
        for (int t: tokens)
            if(t == token)
                return true;
         
        return false;
    }
     
    Input_type get_input(const string &prompt, const string &pattern)
    {
        int utoken = get_token(prompt); // user token
        if (!pattern.empty()) {
            const string delim = "-";
            vector<int> pptokens = parse_pattern(pattern, delim); // parsed pattern tokens
            while(!is_token_match(utoken, pptokens))
                utoken = get_token(prompt);
        }
         
        return static_cast<Input_type>(utoken);
    }
}
