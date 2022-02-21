#ifndef TOKEN_H
#define TOKEN_H

#define NOT_FOUND 404

#include <string>

struct Token {
    int token_id = NOT_FOUND;
    std::string lexeme;
    Token() = default;
    Token(int tid, std::string l) : token_id{ tid }, lexeme{ l } {}
};

/**
 * Note all DFA dunction declarations must have the same signature, i.e.
 * Token <function-name>(std::string::const_iterator& forward, std::string::const_iterator& eol);
 */


Token keywords(std::string::const_iterator& forward, std::string::const_iterator& eol);
Token relop(std::string::const_iterator& forward, std::string::const_iterator& eol);

// add function declarations below

#endif