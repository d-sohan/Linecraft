#include "tokens.h"

Token relop(std::string::const_iterator& forward, std::string::const_iterator& eol) {
    int state = 0;
    char c;
    while (forward != eol) {
        switch (state) {
        case 0:
            c = *forward;
            if (c == '=') {
                ++forward;
                return Token(130, "EQ");
            }
            else if (c == '<') state = 1;
            else if (c == '>') state = 2;
            else if (c == '!') state = 3;
            else return Token();  // not relop
            ++forward;
            break;

        case 1:
            c = *forward;
            if (c == '=') {
                ++forward;
                return Token(121, "LE");
            }
            else return Token(120, "LT");
            break;


        case 2:
            c = *forward;
            if (c == '=') {
                ++forward;
                return Token(111, "GE");
            }
            else return Token(110, "GT");
            break;


        case 3:
            c = *forward;
            if (c == '=') {
                ++forward;
                return Token(131, "NE");
            }
            else {
                --forward;
                return Token(); // ! not relop
            }
            break;
        }
    }
    switch (state) {
    case 1:
        return Token(120, "LT");
        break;
    case 2:
        return Token(110, "GT");
        break;
    default:
        return Token();
    }
}

Token keywords(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    // for now keywords only matches "int"

    int state = 0;
    char c;
    while (forward != eol) {
        switch (state) {
        case 0:
            c = *forward;
            if (c == 'i') state = 1;
            else return Token();
            forward++;
            break;
        case 1:
            c = *forward;
            if (c == 'n') state = 2;
            else return Token();
            forward++;
            break;
        case 2:
            c = *forward;
            if (c == 't') state = 3;
            else return Token();
            forward++;
            break;
        case 3:
            c = *forward;
            if (!isalnum(c) && c != '_') return Token(200, "int");
            else return Token();
            break;
        }
    }
    switch (state) {
    case 3:
        return Token(200, "int");
    default:
        return Token();
    }
}