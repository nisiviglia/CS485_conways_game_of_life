#include <string>
#include <iostream>
#include <cctype>
#include <map>
#include <fstream>

#include "../include/polylex.h"


using namespace std;


/*******************
GET TOKEN
*******************/
Token getToken(istream &in){
    enum LexState { BEGIN, INSTR, ININT, INCOMMENT, INID } state = BEGIN;

	int ch;
	string	lexeme;

	while(  (ch = in.get()) != EOF  ) {

        switch(state){
            case BEGIN:
                if(ch == '\n' || isspace(ch)){
                    continue;
                }

                if(ch == '"'){
                    state = INSTR;
                    break;
                }
                if( ch == '/' && in.peek() == '/'){
                    state = INCOMMENT;
                    break;
                }

                lexeme = ch;
                
                switch(ch){
                    case ',':
                        return Token(COMMA, lexeme);
                    case ':':
                        return Token(COLON, lexeme);
                    // case 'x':
                    //     return Token(EX, lexeme);
                }

                if(isalpha(ch)){
                    state = INID;
                    continue;
                }
                if(isdigit(ch)){
                    state = ININT;
                    continue;
                }
                
                break;
            case ININT:
                //read ints of different forms
                if(isdigit(ch) ){
                    lexeme+=ch;
                    continue;
                }
                in.putback(ch);
                return Token(ICONST, lexeme);
            case INID:
                if( isalpha(ch)){
                    lexeme+=ch;
                    continue;
                }
                in.putback(ch);
                if(lexeme == "name"){
                    return Token(NAME, lexeme);
                }
                if (lexeme == "board"){
                    return Token(BOARD, lexeme);
                }
                if(lexeme == "live"){
                    return Token(LIVE, lexeme);
                }
                if(lexeme == "dead"){
                    return Token(DEAD, lexeme);
                }
                if(lexeme == "toggle"){
                    return Token(TOGGLE, lexeme);
                }
                if(lexeme == "at"){
                    return Token(AT, lexeme);
                }
                if(lexeme == "x"){
                    return Token(EX, lexeme);
                }
                else{
                    return Token(ERROR, lexeme);
                }
                break;
            case INSTR:
                if(ch == '\n'){
                    return Token(ERROR, lexeme);
                }
                if( ch != '"'){
                    lexeme+= ch;
                    continue;
                }
                return Token(STR, lexeme);
            case INCOMMENT: 
                if ( ch!= '\n'){
                    continue;
                }
                state = BEGIN;
                break;
            default:
                cerr<< "Unknown value"<<endl;
                return Token();
        }
    }
    if( state == BEGIN )
		return Token(DONE, "");
	else if( state == ININT )
		return Token(ICONST, lexeme);
	else
		return Token(ERROR, lexeme);
}

/***************
 * WRAPPER
 **************/

// Token saved;
// bool isSaved = false;

// Token GetAToken(istream *in) {
// 	if( isSaved ) {
// 		isSaved = false;
// 		return saved;
// 	}

// 	return getToken(in);
// }
// void PushbackToken(Token& t) {
// 	if( isSaved ) {
// 		cerr << "Can't push back more than one token!!!";
// 		exit(0);
// 	}

// 	saved = t;
// 	isSaved = true;
// }

// /****************************************
// PARSETREE
// ******************************************/

// class ParseTree {
//     private:
//         ParseTree *leftchild;
//         ParseTree *rightchild;
//     public: 
//         ParseTree (ParseTree *left = 0, ParseTree *right = 0) : leftchild(left), rightchild(right){};
//         ParseTree* getLeft(){return leftchild;}
//         ParseTree* getRight(){return rightchild;}
// };

// class CreateWorld : public ParseTree {
//     public: 
//         CreateWorld(ParseTree *, ParseTree *right): ParseTree(left, right){}
// };

// class CreateBoard : public ParseTree{
//     public:
//         CreateBoard(ParseTree *)
// }


/**************
MAIN
**************/
/*int main(int argc, char *argv[]){
    fstream input_file;
    input_file.open(argv[1], ios::in);
    
    if (!input_file.is_open()){
        cerr << "Error, could not open file!\n";
        return 1;
    }

    istream* input = &input_file;

    Token tok;
    Token doneTok(DONE, "");

    while( !((tok =  getToken(input)) == doneTok)){
        cout<<"New Token: "<< tok.getTokenType() << ", with Lex: "<< tok.getLexeme()<<endl;
    }
    input_file.close();
    //start of parseTree
    //ParseTree *script  = Script(input);

    return 0;
}*/



// map<string, bool> IdentifierMap;
// map<string, Value> symbolMap;
// void printPostorder(ParseNode *node){
//     static int count = 0;
//     // cout <<"Count: " << count++ << endl;
//     /*if (node == 0){
//         return;
//     }*/
    
//     node->RunStaticChecks(IdentifierMap);
//     node->Eval(symbolMap);
// }
