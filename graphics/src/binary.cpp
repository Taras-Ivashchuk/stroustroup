/*
11. Define a Binary_tree class derived from Shape. Give the number of levels as a parameter (levels==0 means no
nodes, levels==1 means one node, levels==2 means one top node with two sub-nodes, levels==3 means one top node
with two sub-nodes each with two sub-nodes, etc.). Let a node be represented by a small circle. Connect the nodes by
lines (as is conventional). P.S. In computer science, trees grow downward from a top node (amusingly, but logically,
often called the root).
13. Modify Binary_tree to take a parameter (or parameters) to indicate what kind of line to use to connect the nodes (e.g.,
an arrow pointing down or a red arrow pointing up). Note how this exercise and the last use two alternative ways of
making a class hierarchy more flexible and useful.
14. Add an operation to Binary_tree that adds text to a node. You may have to modify the design of Binary_tree to
implement this elegantly. Choose a way to identify a node; for example, you might give a string "lrrlr" for navigating left,
right, right, left, and right down a binary tree (the root node would match both an initial l and an initial r).
*/

#include "Graph.h"
#include "Simple_window.h"
#include "std_lib_facilities.h"


using namespace Graph_lib;

class Binary_tree: public Shape {
	public:
		Binary_tree(int llevels, Line_style::Line_style_type llstyle): levels{ llevels }, lstyle { llstyle }
	       	{
			add_nodes();
			add_lines();
		}
		void draw_lines() const;
		void set_text(string path, string text);
	private:
		int levels{};
		int r{ 40 };
		Line_style lstyle;
		int get_index(string s) const;
		Point get_offset(int, int) const;
		vector<Point> total_nodes;
		Lines total_lines;
		void add_nodes();
		void add_lines();
		void copy_nodes(vector<Point> &to, vector<Point> &from);
		void draw_text() const;
		vector<pair<string, string>> path_text; // pairs: Path to node in format "llrl"(root->left->left...), node text
};

Point Binary_tree::get_offset(int  width, int height) const
{
		double hypotenuse{ sqrt(pow(height, 2) + pow(width, 2))};
		double sinus{ height / hypotenuse };
		double cosinus{ width / hypotenuse };
		Point offsetxy{ r * sinus, r * cosinus };
		return offsetxy;
}

void Binary_tree::copy_nodes(vector<Point> &to, vector<Point> &from)
{
	to.clear();
	for(Point p: from)
		to.push_back(p);
}

void Binary_tree::add_nodes()
{
	int height{ 4 * r }; // height between parent and child
	int width{};         // width between parent and child
	vector<Point> prev_nodes;
	for (int i = 0; i < levels; ++i) {
		vector<Point> current_nodes;
		if (i == 0) { // handle the root node
			Point root{ 900, 100 };
			total_nodes.push_back(root);
			current_nodes.push_back(root);
		}				     // deal with children
		if (i != 0)
			width = 8 * r / i;
		for (Point pp: prev_nodes) { // previous level points 
					     // for each point of previous level we 
					     // add left child
			Point center_left{ pp.x - width, pp.y + height };
			total_nodes.push_back(center_left);
			current_nodes.push_back(center_left);
			// add right child
			Point center_right{ pp.x + width, pp.y + height };
			total_nodes.push_back(center_right);
			current_nodes.push_back(center_right);
		}
		copy_nodes(prev_nodes, current_nodes);
	}
}

void Binary_tree::add_lines()
{
	int height{ 4 * r };
	int width{};
	for (int i = 0; i < pow(2, levels - 1) - 1; ++i) {// iterate levels excluding last level
		width = 12 * r / (i + 1);
		Point pp = total_nodes[i];		// parent point
		Point offsetxy { get_offset(width, height) };
		// add left line
		Point left_ch = total_nodes[i * 2 + 1]; // left child
		Point start{ pp.x - offsetxy.x, pp.y + offsetxy.y };
		Point end{ left_ch.x + offsetxy.x, left_ch.y - offsetxy.y };
		Line left_line{ start, end };
		total_lines.add(start, end);
		// add right line
		Point right_ch = total_nodes[i * 2 + 2]; // right child
		start = { pp.x + offsetxy.x, pp.y + offsetxy.y };
		end = { right_ch.x - offsetxy.x, right_ch.y - offsetxy.y };
		Line right_line{ start, end };
		total_lines.add(start, end);
	}
	total_lines.set_style(lstyle);
}

int Binary_tree::get_index(string s) const
{
	int multipl = 2;
	int prev{}; 
	for (int i = 0; i < s.size(); ++i) {
		if (i == 0 && (s[i] == 'l' || s[i] == 'r'))
			prev = 0;
		else if (s[i] == 'l')
			prev = prev * multipl + 1;
		else if (s[i] == 'r')
			prev = prev * multipl + 2;
	}

	return prev;
}

/*
write text to node indictated by path,
path is defined in following way, "lrrlr" ,
means l or r start from the top, then right,
then right, then go left, ther right
*/
void Binary_tree::set_text(string ppath, string ttext)  
{						      
	pair <string, string> p(ppath, ttext);
	path_text.push_back(p);
}

void Binary_tree::draw_text() const
{
	if (path_text.size() == 0)
		return;

	if (path_text.size() > levels)		  
		return;

	for (pair<string,string> pt: path_text) {
		int idx = get_index(pt.first);
		Point center{ total_nodes[idx] };
		Text t { center, pt.second };
		t.draw_lines();
	}
}


void Binary_tree::draw_lines() const 
{
	// draw nodes
	for (Point p: total_nodes) {
		Circle c{p, r};
		c.draw_lines();
	}

	// draw lines
	total_lines.draw();

	// draw text
	draw_text();
}

int main()
{
	Point topleft{ 300, 100 };
	int wwidth{ 1200 };
	int wheight{ 900 };
	string title{ "Binary tree" };
	Simple_window sw{ topleft, wwidth, wheight, title }; 
	Binary_tree bt{4, Line_style::dot};
	bt.set_text("lrlr", "world"); 
	bt.set_text("l", "hello"); 
	sw.attach(bt);
	sw.wait_for_button();
}
