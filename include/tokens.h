#ifndef TOKEN_H
#define TOKEN_H


// Token_id symbolic constants

#define NOT_FOUND 404

#define ID 500

#define KWD_BASE 300

#define LE 121
#define GE 111
#define NE 131
#define LT 120
#define GT 110
#define EQ 130

#define OR 150
#define AND 145
#define NOT 140

#define ASSIGN 160

#define ADD 170
#define SUB 171
#define MUL 172
#define DIV 173
#define MOD 174

#define NUM_I 610
#define NUM_0 600
#define NUM_F 650

#define CHAR_LIT 700
#define CHAR_LIT_UKE 709
#define UNTER_CHAR 711
#define MUL_CHAR 715
#define EMPTY_CHAR 719
#define STR_LIT 750
#define STRL_LIT_UKE 759
#define UNTER_STR 761

#define OPEN_ROUND 200
#define OPEN_CURLY 202
#define OPEN_SQUARE 204
#define CLOSE_ROUND 201
#define CLOSE_CURLY 203
#define CLOSE_SQUARE 205

#define COMMA 210
#define SEMICOLON 211 




#include <string>
#include <vector>

extern std::vector<std::string> keywords;

struct Token {
    int token_id = NOT_FOUND;
    std::string lexeme;
    Token() = default;
    Token(int tid) : token_id{ tid } {}
    Token(int tid, std::string l) : token_id{ tid }, lexeme{ l } {}
};

/**
 * Note all DFA dunction declarations must have the same signature, i.e.
 * Token <function-name>(std::string::const_iterator& forward, std::string::const_iterator& eol);
 */



Token kid(std::string::const_iterator& forward, std::string::const_iterator& eol);

Token op(std::string::const_iterator& forward, std::string::const_iterator& eol);

Token delim(std::string::const_iterator& forward, std::string::const_iterator& eol);

Token num(std::string::const_iterator& forward, std::string::const_iterator& eol);

Token sclit(std::string::const_iterator& forward, std::string::const_iterator& eol);


// add function declarations below

#endif