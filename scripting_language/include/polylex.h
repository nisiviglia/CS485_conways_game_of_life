#ifndef POLYLEX_H_
#define POLYLEX_H_

#include <string>
#include <iostream>

using namespace std;

/*******************
TOKENS
*******************/

enum TokenType{
    NAME,
    BOARD,
    LIVE,
    DEAD,
    TOGGLE,
    //OBJECT,
    ICONST,
    STR,
    COLON,
    EX,
    COMMA,
    LP,
    RP,
    AT,
    ERROR,
    DONE
};

class Token {
    private:
        TokenType tok;
        string lexeme;
        
    public:
        Token() : tok(ERROR), lexeme(""){}
        Token (TokenType t, string s) : tok(t), lexeme(s){}

        TokenType getTokenType() const {return tok;}
        string getLexeme() const {return lexeme;} 

        friend bool operator==(const Token& left, const Token& right) {
		    return left.tok == right.tok;
	    }
};
 
extern Token getToken(istream &in);

#endif