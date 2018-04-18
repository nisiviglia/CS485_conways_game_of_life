// Derived from gwryan's ParseNode.h (CS280)

// #define DEBUG_PRINT	//Debug print for ParseNode.h 

#ifndef PARSENODE_H_
#define PARSENODE_H_

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

using namespace std;
#include "polylex.h"

#include "ObjDispatcher.hpp"

extern int globalErrorCount;
extern vector<pair<int,int>> live_cells;
extern string world_name;

class ParseNode {
protected:
	ParseNode *left;
	ParseNode *right;
public:
	ParseNode (ParseNode *left=0, ParseNode *right=0) : left(left), right(right) {}

	//	virtual destructor
	virtual ~ParseNode() {}

	virtual void RunStaticChecks(std::map<string,bool> &idMap){
		if ( left )
			left->RunStaticChecks(idMap);
		if ( right )
			right->RunStaticChecks(idMap);
	}

	// virtual Value Eval(){}
	virtual void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "Main ParseNode" << endl;
		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);

	}

	virtual void getPairVector(ParseNode * init, std::vector<std::pair<int,int>> &vector_pair){}

	virtual std::pair<int, int> getPair() {}
};

class InitScript: public ParseNode {
public:
	InitScript(ParseNode *l, ParseNode *r) : ParseNode(l,r) {}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "InitScript" << endl;
		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);

		// ADD LIVE CELLS TO OBJLIB (API CALLS)
		int frameId = 1;
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		int rc = dispatcher->Single(ol::INSERT, ol::FRAME, 1, world_name, &live_cells);
#ifdef DEBUG_PRINT
		if (!rc) {
			cout << "Live cells to be added to world '" << world_name << "': " << endl;
			for (int i = 0; i < live_cells.size(); i++) {
				cout << live_cells[i].first << "," << live_cells[i].second << " ";
			}
			cout << endl;
		} else {
			cout << "Frame could not be inserted into world: " << world_name << endl;
			cout << dispatcher->GetErrorLog() << endl;
		}
#endif
		delete(dispatcher);
	}
};

class Environment: public ParseNode {
public:
	Environment(ParseNode *l) : ParseNode(l) {}

	void Action(std::vector<string> &loc) {

		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);
	}
};

class CreateWorld: public ParseNode {
	// string worldName;
public:
	CreateWorld(string w) : ParseNode() {
		// worldName = w;
		world_name = w;
	}

	void Action(std::vector<string> &loc) {
		
		//API CALLS TO OBJLIB
		ol::ObjDispatcher *dispatcher = new ol::ObjDispatcher();
		int rc = dispatcher->Single(ol::INSERT, ol::WORLD, 0, world_name, NULL);
#ifdef DEBUG_PRINT
		if (!rc) {
			cout << "Created world: " << world_name << endl;
		} else {
			cout << "Could not create world: " << world_name << endl;
			cout << dispatcher->GetErrorLog() << endl;
		}
#endif
		delete(dispatcher);
	}
};

class CreateBoard: public ParseNode {
public:
	// int dim1 = 0, dim2 = 0;
	CreateBoard(ParseNode *l) : ParseNode(l) {
		// dim1 = d1;
		// dim2 = d2;
	}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "CreateBoard" << endl;
		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);
	}
};

//command to the left and a list of commands to the right
class CommandList: public ParseNode {
public:
	CommandList(ParseNode *l, ParseNode *r) : ParseNode(l,r) {}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "CommandList" << endl;
		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);
	}
};

//	for INDIVIDUAL CoordinateFormat -> cell_pairs.size() == num pairs
//	for RANGE_MODE1 & RANGE_MODE2 -> cell_pairs.size() == 2

enum CommandType{
	LIVE_CMD,
	DEAD_CMD,
	TOGGLE_CMD,
};

enum CommandFormat{
	INDIVIDUAL,
	RANGE
};

void addtoLiveCells(vector<pair<int,int>> &pair_vector);
void deadfromLiveCells(vector<pair<int,int>> &pair_vector);
void toggleLiveCells(vector<pair<int,int>> &pair_vector);

class Command: public ParseNode {
protected:
	std::vector<ParseNode *> pairs;
	CommandType cmd_type;
	CommandFormat cmd_form;
public:
	Command(std::vector<ParseNode *> cell_pairs, CommandType cmd, CommandFormat form) : 
											pairs(cell_pairs), cmd_type(cmd), cmd_form(form), ParseNode() {}

	void Action(std::vector<string> &loc) {
		// cout << "LiveCommand" << endl;
		std::vector<std::pair<int, int>> pair_vector;

		if (pairs.size() == 2 && cmd_form == RANGE) { // range
			pairs[1]->getPairVector(pairs[0], pair_vector);
		}
		else {
			for (int i = 0; i < pairs.size(); i++) {
				pair_vector.push_back(pairs[i]->getPair());
			}
		}

		// UPDATE live_cells vector
		switch (cmd_type) {
			case LIVE_CMD: 
				addtoLiveCells(pair_vector); 
				break;
			case DEAD_CMD: 
				deadfromLiveCells(pair_vector);
				break;
			case TOGGLE_CMD: 
				toggleLiveCells(pair_vector);
				break;
		}
	}
};

class Coordinate: public ParseNode {
public:
	int dim1, dim2;
	Coordinate(int d1, int d2) : ParseNode() {
		dim1 = d1;
		dim2 = d2;
	}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "Coordinate" << endl;
		
	}

	void getPairVector(ParseNode * init, std::vector<std::pair<int,int>> &vector_pair){
		std::pair<int, int> initial_pair = init->getPair();

		for (int i = initial_pair.first; i <= dim1; i++) {
			for (int j = initial_pair.second; j <= dim2; j++) {
				std::pair<int, int> tmp (i, j);
				vector_pair.push_back(tmp);
			}
		}
	}

	std::pair<int, int> getPair() {
		std::pair<int, int> point (dim1, dim2);
		return point;
	}
};

class Range: public ParseNode {
public:
	int dim1, dim2;
	Range(int d1, int d2) : ParseNode() {
		dim1 = d1;
		dim2 = d2;
	}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "Range" << endl;
		
	}

	void getPairVector(ParseNode *init, std::vector<std::pair<int,int>> &vector_pair) {
		std::pair<int, int> initial_pair = init->getPair();
		int initFirst = initial_pair.first;
		int initSecond = initial_pair.second;

		for (int i = 0; i < dim1; i++) {
			for (int j = 0; j < dim2; j++) {
				std::pair<int, int> tmp (i + initFirst, j + initSecond);
				vector_pair.push_back(tmp);
			}
		}
	}
};

// a list of commands is represented by a command on left and list of commands to right
class CmdList: public ParseNode {
public:
	CmdList(ParseNode *l, ParseNode *r): ParseNode(l,r) {}

	void Action(std::vector<string> &loc) {
		// loc.push_back("Main ParseNode");
		// cout << "CmdList" << endl;
		if (left)
			left->Action(loc);
		if (right)
			right->Action(loc);
	}
};



extern ParseNode *Script(istream &in);
extern ParseNode *Env(istream &in);
extern ParseNode *World(istream &in);
extern ParseNode *Board(istream &in);
extern ParseNode *Stmt(istream &in);
extern ParseNode *Cmd(istream &in);
extern ParseNode *Coord(istream &in);
extern ParseNode *CoordOrRange(istream &in);

#endif /* PARSENODE_H_ */














