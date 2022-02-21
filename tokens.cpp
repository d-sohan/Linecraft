#include "tokens.h"

/**
 * Note function definitons must return a Token and take the same parameters, namely
 * std::string::const_iterator& forward, std::string::const_iterator& eol
 */



 // Add new DFA functions below 



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
            if (!isalnum(c) && c != '_') return Token(300, "int");
            else return Token();
            break;
        }
    }
    switch (state) {
    case 3:
        return Token(300, "int");
    default:
        return Token();
    }
}

Token iden(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    char c;
    std::string idx;
    if (forward == eol || !isalpha(*forward)) return Token();
    else {
        idx = *forward;
        ++forward;
    }
    while (forward != eol) {
        c = *forward;
        if (isalnum(c) || c == '_') {
            idx += c;
            ++forward;
        }
        else break;
    }
    return Token(500, idx);
}

Token num(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    if (forward == eol || !isdigit(*forward)) return Token();
    std::string idx;
    int state = 0;
    if (*forward == '0') {
        ++forward;
        if (*forward == '.') {
            idx = "0.";
            ++forward;
            state = 2;
        }
        else return Token(600, "0");
    }
    else {
        idx = *forward;
        ++forward;
        state = 1;
    }
    char c;
    while (forward != eol) {
        c = *forward;
        if (isdigit(c)) {
            idx += c;
            ++forward;
        }
        else if (state == 1 && c == '.') {
            state = 2;
            idx += c;
            ++forward;
        }
        else break;
    }
    if (state == 1) return Token(610, idx);
    else return Token(650, idx);
}


Token op(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    if (forward == eol) return Token();
    int state = 0;
    ++forward;
    switch (*(forward - 1)) {
    case '=':
        return Token(130, "=");
    case '+':
        return Token(170, "+");
    case '-':
        return Token(171, "-");
    case '*':
        return Token(172, "*");
    case '/':
        return Token(173, "/");
    case '%':
        return Token(174, "%");
    case '<':
        state = 1;
        break;
    case '>':
        state = 2;
        break;
    case '!':
        state = 3;
        break;
    case '&':
        state = 4;
        break;
    case '|':
        state = 5;
        break;
    case ':':
        state = 6;
        break;
    default:
        --forward;
        return Token();
    }
    switch (state) {
    case 1:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(121, "<=");
        }
        else return Token(120, "<");
        break;
    case 2:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(111, ">=");
        }
        else return Token(110, ">");
        break;
    case 3:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(131, "!=");
        }
        else return Token(140, "!");
        break;
    case 4:
        if (forward != eol && *forward == '&') {
            ++forward;
            return Token(145, "&&");
        }
        else {
            --forward;
            return Token();
        }
        break;
    case 5:
        if (forward != eol && *forward == '|') {
            ++forward;
            return Token(150, "||");
        }
        else {
            --forward;
            return Token();
        }
        break;
    case 6:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(160, ":=");
        }
        else {
            --forward;
            return Token();
        }
        break;
    default:
        return Token();
    }
}

Token delim(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    int state = 0;
    char c = *forward;
    ++forward;
    switch (c) {
    case '(':
        return Token(200, "(");
    case ')':
        return Token(201, ")");
    case '{':
        return Token(202, "{");
    case '}':
        return Token(203, "}");
    case '[':
        return Token(204, "[");
    case ']':
        return Token(205, "]");
    case ',':
        return Token(210, ",");
    case ';':
        return Token(211, ";");
    default:
        --forward;
        return Token();
    }
}