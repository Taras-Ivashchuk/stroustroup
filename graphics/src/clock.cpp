//  Make an “analog clock,” that is, a clock with hands that move. You get the time of day from the operating system through
// a library call. A major part of this exercise is to find the functions that give you the time of day and a way of waiting for a
// short period of time (e.g., a second for a clock tick) and to learn to use them based on the documentation you found. Hint:
// clock(), sleep().
 
#include "GUI.h"
#include "Graph.h"
#include <functional>
#include "std_lib_facilities.h"
#include <ctime>
#include <unistd.h>
#include <cstdlib>

using namespace Graph_lib;
 
// to make Fl::wait() return immitiately
void cb_idle(void* data) {}  
 
// Function to convert degrees to radians
double deg_to_rad(double degrees) {
    return degrees * (M_PI / 180.0); 
}

class My_clock : public Window {
    public:
	My_clock(Point xy, int w, int h, const string title);
    void go();

    private:
	Button quit_btn;
    Circle circle;
    Lines hours;   // hour notches
    Lines minutes; // minute notches
    Point center;
    Vector_ref<Text> hour_labels;
    double chour{}; // current hour
    double cminutes{}; // current minutes
    double cseconds{}; // current seconds
     
	static void cb_quit(Address, Address);
	void quit();
    void draw_hours();
    void draw_minutes();
    void draw_hour_hand(Lines &);
    void draw_minutes_hand(Lines &);
    void draw_seconds_hand(Lines &);
    void get_current_time();
    void update_time();
};

void My_clock::update_time()
{
        if (cseconds == 60) {
            cseconds = 0;
            cminutes ++;
        }
        if (cminutes == 60) {
            cminutes = 0;
            chour ++;
        }
        if (chour == 12)
            chour = 0;
}

void My_clock::go()
{
    get_current_time();

    // register idle calback so wait() returns immidiately
    Fl::add_idle(cb_idle);
     
    while(true) {
        update_time();
        Lines seconds_hand; // reinitialize so only one object to be attached
        Lines hour_hand;
        Lines minutes_hand;
	    draw_hour_hand(hour_hand);
	    draw_minutes_hand(minutes_hand);
	    draw_seconds_hand(seconds_hand);
	    attach(hour_hand);
	    attach(minutes_hand);
	    attach(seconds_hand);
	    Fl::redraw(); // draw it to the screen!
        Fl::wait();
	    sleep(1);
        cseconds ++;
	    detach(seconds_hand);
        detach(minutes_hand);
        detach(hour_hand);
    }
}

void My_clock::get_current_time()
{
    time_t timestamp = time(NULL);
    struct tm datetime = *localtime(&timestamp);
    chour = double(datetime.tm_hour);
    cminutes = double(datetime.tm_min);
    cseconds = double(datetime.tm_sec);
}
 
void My_clock::draw_seconds_hand(Lines &seconds_hand)
{
    int r = circle.radius();

    // start rotating clockwise
    double start_deg = 90.0;
    constexpr int dist = 6;
    double target_deg = start_deg - cseconds * dist;
    double xoffset = center.x + cos(deg_to_rad(target_deg)) * r * 2 / 3;
    double yoffset = center.y - sin(deg_to_rad(target_deg)) * r * 2 / 3;
    Point p{ xoffset, yoffset };
    seconds_hand.add(center, p);
    seconds_hand.set_style(Line_style{Line_style::solid, 5});
}
 
void My_clock::draw_minutes_hand(Lines& minutes_hand)
{
    int r = circle.radius();
     
    // start rotating clockwise
    double start_deg = 90.0;
    constexpr int dist = 6;
    double target_deg = start_deg - (cminutes + cseconds / 60) * dist;
    double xoffset = center.x + cos(deg_to_rad(target_deg)) * r * 2 / 3;
    double yoffset = center.y - sin(deg_to_rad(target_deg)) * r * 2 / 3;
    Point p{ xoffset, yoffset };
    minutes_hand.add(center, p);
    minutes_hand.set_style(Line_style{Line_style::solid, 20});
}

void My_clock::draw_hour_hand(Lines& hour_hand) 
{
    int r = circle.radius();
     
    // start rotating clockwise
    double start_deg = 90.0;
    constexpr int dist = 30;
    // add minutes for more precision
    double target_deg = start_deg - (chour + cminutes / 60) * dist;

    double xoffset = center.x + cos(deg_to_rad(target_deg)) * r * 1 / 2.5;
    double yoffset = center.y - sin(deg_to_rad(target_deg)) * r * 1 / 2.5;
    Point p{ xoffset, yoffset };
    hour_hand.add(center, p);
    hour_hand.set_style(Line_style{Line_style::solid, 20});
}

void My_clock::draw_minutes()
{
    double deg = 90.0; 
    int r = circle.radius();
    for (int i = 0; i < 60; ++i) {
        // inner point within circle
        double xoffset = center.x + cos(deg_to_rad(deg)) * r * 7/8;
        double yoffset = center.y - sin(deg_to_rad(deg)) * r * 7/8;
        Point a{xoffset, yoffset}; 
         
        // point lies on the circle
        xoffset = center.x + cos(deg_to_rad(deg)) * r;
        yoffset = center.y - sin(deg_to_rad(deg)) * r;
        Point b{xoffset, yoffset}; 
        minutes.add(a, b);
        deg -= 6;
    }
}

void My_clock::draw_hours()
{
    double deg = 90.0; 
    int r = circle.radius();
    for (int i = 0; i < 12; ++i) {
        // inner point within circle
        double xoffset = center.x + cos(deg_to_rad(deg)) * r * 5/6;
        double yoffset = center.y - sin(deg_to_rad(deg)) * r * 5/6;
        Point a{xoffset, yoffset}; 
         
        // point lies on the circle
        xoffset = center.x + cos(deg_to_rad(deg)) * r;
        yoffset = center.y - sin(deg_to_rad(deg)) * r;
        Point b{xoffset, yoffset}; 
         
        hours.add(a, b);
        hours.set_style(Line_style{Line_style::solid, 4});
         
        // points for text labels
        xoffset = center.x + cos(deg_to_rad(deg)) * r * 3/4;
        yoffset = center.y - sin(deg_to_rad(deg)) * r * 3/4;
        Point c{xoffset - 25, yoffset + 25}; // add some space around
         
        ostringstream oss;
        if (i == 0) {
            oss << 12;
            c.x -= 20; // to nicely place 12 in middle of notch
        } else
            oss << i;

        hour_labels.push_back(new Text{Point{c.x, c.y}, oss.str()});
        hour_labels[i].set_font_size(80);
         
        deg -= 30;
    }
}

My_clock::My_clock(Point xy, int w, int h, const string title)
	: Window{ xy, w, h, title }
	, quit_btn{ Point{ x_max() - 70, 0 }, 70, 20, "Quit", cb_quit }
	, circle{ Point{ x_max() / 2, y_max() / 2 }, y_max() / 2 }
{
    center = circle.center();
    attach(quit_btn);
    attach(circle);
    draw_minutes();
    attach(minutes);
    draw_hours();
    attach(hours);
    for (int i = 0; i < hour_labels.size(); ++i)
        attach(hour_labels[i]);
}

void My_clock::cb_quit(Address, Address pw)
{
	reference_to<My_clock>(pw).quit();
}

void My_clock::quit()
{
    exit(0);
}

int main()
{
	My_clock mc{ Point{ 100, 100 }, 1200, 900, "clock" };
    mc.go();
}
