// Derived from gwryan's ParseNode.h (CS280)

#include "../include/ParseNode.h"

static bool pushedBack = false;
static Token pushedToken;

//**********************************************************
// wraper around polylex's gettoken function, making it compatible with the new PutBackToken
Token GetToken(istream &in) {
	if (pushedBack) {
		pushedBack = false;
		// cout << "got: " << pushedToken.getLexeme() << endl;
		return pushedToken;
	}

	// call gettoken.cpp:gettoken()
	Token r = getToken(in);
	// cout << "got: " << r.getLexeme() << endl;
	return r;
}

//**********************************************************
void PutBackToken(Token &t) {
	if (pushedBack){
		cout << "Can't push back another token!" << endl;
		exit(0);
	}

	// cout << "Pushback: " << t.getLexeme() << endl;

	pushedBack = true;
	pushedToken = t;
}

//**********************************************************
void error (string s) {
	cout << "PARSE ERROR: ";
	cout << s << endl;
	globalErrorCount++;
}

//**********************************************************
ParseNode *Script(istream &in) {
	ParseNode *env = Env(in);

	if (env != 0) {
		return new InitScript(env, Stmt(in));
	}

	return 0;
}

//**********************************************************
ParseNode *Env(istream &in) {
	ParseNode *world = World(in);

	if (world != 0) {
		// return new Environment(world, Board(in));
		// ParseNode *board = Board(in);
		return new Environment(world);
	}

	return 0;
}

//**********************************************************
//	CREATE_WORLD := name “WORLD_NAME” CREATE_BOARD
ParseNode *World(istream &in) {
	Token cmd = GetToken(in);
	if (cmd.getTokenType() == NAME) {
		Token worldName = GetToken(in);
		if (worldName.getTokenType() == STR){
			return new CreateWorld(worldName.getLexeme());
		}
		else{
			error("worldName required after 'name' keyword.");
			return 0;
		}
	}
	else{
		error("The first command should must be initializing new world.");
		return 0;
	}

	//should never get here
	return 0;
}

//**********************************************************
//	CREATE_BOARD := board COORD
ParseNode *Board(istream &in) {
	Token cmd = GetToken(in);
	if (cmd.getTokenType() == BOARD) {
		ParseNode *coord = Coord(in);
		if (coord != 0){
			return new CreateBoard(coord);
		}
	}
	else {
		// board dimension not specified
		//	create one with default dimensions 100x100
		PutBackToken(cmd);
		ParseNode *coord = new Coordinate(100, 100);
		return new CreateBoard(coord);
	}
}

//**********************************************************
//	STMT := CMD | CMD STMT
ParseNode *Stmt(istream &in) {
	ParseNode *cmd = Cmd(in);

	if (cmd != 0){
		return new CommandList(cmd, Stmt(in));
	}

	return 0;
}

//**********************************************************
int updateActionCellVector(std::vector<ParseNode *> &cell_pairs, istream &in){

	//check for at AT keyword
	Token cmd = GetToken(in);
	if (cmd.getTokenType() != AT) {
		error ("Missing 'at' keyword."); 
		return 0;
	}

	ParseNode *coord = Coord(in);

	if (coord) {
		cell_pairs.push_back(coord);

		//	check COLON first
		cmd = GetToken(in);
		if (cmd.getTokenType() == COLON) {
			//	:COORD or :RANGE
			coord = CoordOrRange(in);
			if (coord) {
				cell_pairs.push_back(coord);
			}
			else {
				return 0;
			}
			return 2;
		}
		else {
			PutBackToken(cmd);
			//	implement [COORD ]+
			while (coord = Coord(in)) {
				cell_pairs.push_back(coord);
			}
			return 1;
		}	
	}
	else {
		error ("Invalid first coordinate on cell command.");
		return 0;
	}

	//	Shouldn't get here
	return 1; 
}

//**********************************************************
//	CMD := LIVE_CMD | DEAD_CMD | TOGGLE_CMD
ParseNode *Cmd(istream &in) {
	Token cmd = GetToken(in);
	TokenType cmd_tok = cmd.getTokenType();
	//check if valid command
	if (cmd_tok != LIVE && cmd_tok != DEAD && cmd_tok != TOGGLE) {
		PutBackToken(cmd); 
		return 0;
	}

	// struct ActionCell *action_struct = (struct ActionCell *)malloc(sizeof(struct ActionCell));
	std::vector<ParseNode *> cell_pairs;
	int rc = updateActionCellVector(cell_pairs, in);

	switch (rc) {
		case 1: switch (cmd_tok) {
					case LIVE:		return new Command(cell_pairs, LIVE_CMD, INDIVIDUAL);
					case DEAD:		return new Command(cell_pairs, DEAD_CMD, INDIVIDUAL);
					case TOGGLE:	return new Command(cell_pairs, TOGGLE_CMD, INDIVIDUAL);
					default: 		return 0;
				}
		case 2:	switch (cmd_tok) {
					case LIVE:		return new Command(cell_pairs, LIVE_CMD, RANGE);
					case DEAD:		return new Command(cell_pairs, DEAD_CMD, RANGE);
					case TOGGLE:	return new Command(cell_pairs, TOGGLE_CMD, RANGE);
					default: 		return 0;
				}
		default: return 0;
	}
}

//**********************************************************
void coord_error() {
	error ("Invalid coordinate.");
}

//**********************************************************
//	COORD := ICONST,ICONST
ParseNode *Coord(istream &in) {
	Token cmd = GetToken(in);
	if (cmd.getTokenType() != ICONST) {
		PutBackToken(cmd); 
		return 0;
	}
	int dim1 = stoi(cmd.getLexeme());

	cmd = GetToken(in);
	if (cmd.getTokenType() != COMMA) {
		coord_error(); 
		return 0;
	}

	cmd = GetToken(in);
	if (cmd.getTokenType() != ICONST) {
		coord_error(); 
		return 0;
	}
	int dim2 = stoi(cmd.getLexeme());

	return new Coordinate(dim1, dim2);
}

//**********************************************************
//	RANGE := ICONSTxICONST
ParseNode *CoordOrRange(istream &in) {
	Token cmd = GetToken(in);
	if (cmd.getTokenType() != ICONST) {
		coord_error; return 0;
	}
	int dim1 = stoi(cmd.getLexeme());

	cmd = GetToken(in);
	if (cmd.getTokenType() == COMMA){		//	coordinate
		cmd = GetToken(in);
		if (cmd.getTokenType() == ICONST) {
			int dim2 = stoi(cmd.getLexeme());
			return new Coordinate(dim1, dim2);
		}
		else {
			coord_error(); 
			return 0;
		}
	}
	else if (cmd.getTokenType() == EX) { 	//	range
		cmd = GetToken(in);
		if (cmd.getTokenType() == ICONST) {
			int dim2 = stoi(cmd.getLexeme());
			return new Range(dim1, dim1);
		}
		else {
			error("Invalid range."); 
			return 0;
		}
	}
	else {
		error ("Invalid argument entered inside cell action command.");
		return 0;
	}

	//should never get here
	return 0;
}

//**********************************************************
//*	Appending to global live_cells vector
void addtoLiveCells(vector<pair<int,int>> &pair_vector) {
	for (int i = 0; i < pair_vector.size(); i++) {
		if (std::find(live_cells.begin(), live_cells.end(), pair_vector[i]) != live_cells.end() ) {
			// pair already exists in live_cells
			// so don't append again
		}
		else {
			//	pair doesn't exist 
			live_cells.push_back(pair_vector[i]);
#ifdef DEBUG_PRINT
			cout << "Live: " << pair_vector[i].first << "," << pair_vector[i].second << endl;
#endif
		}
	}
}

//**********************************************************
//*	Removing from live_cells vector
void deadfromLiveCells(vector<pair<int,int>> &pair_vector) {
	for (int i = 0; i < pair_vector.size(); i++) {
		for (int j = 0; j < live_cells.size(); j++) {
			if (pair_vector[i] == live_cells[j]) {
#ifdef DEBUG_PRINT
				cout << "Dead: " << pair_vector[i].first << "," << pair_vector[i].second << endl;
#endif
				live_cells.erase(live_cells.begin() + j);
				break;
			}
		}
	}
}

//**********************************************************
void toggleLiveCells(vector<pair<int,int>> &pair_vector) {
	bool found;
	for (int i = 0; i < pair_vector.size(); i++) {
		found = false;
		for (int j = 0; j < live_cells.size(); j++) {
			if (pair_vector[i] == live_cells[j]) {
#ifdef DEBUG_PRINT
				cout << "Toggle Dead: " << pair_vector[i].first << "," << pair_vector[i].second << endl;
#endif
				live_cells.erase(live_cells.begin() + j);
				found = true;
				break;
			}
		}
		if (!found) {
#ifdef DEBUG_PRINT
				cout << "Toggle Live: " << pair_vector[i].first << "," << pair_vector[i].second << endl;
#endif
			live_cells.push_back(pair_vector[i]);
		}
	}
}
















