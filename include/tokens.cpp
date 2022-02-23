#include "tokens.h"
#include "utilities.h"



/**
 * DFA Functions are implemented here
 *
 * Note: function definitons must return a Token and take the same parameters, namely
 *     std::string::const_iterator& forward, std::string::const_iterator& eol
 */





Token kid(std::string::const_iterator& forward, std::string::const_iterator& eol)
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

    return Token(check_keywords(idx), idx);
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
        else return Token(NUM_0, "0");
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
    if (state == 1) return Token(NUM_I, idx);
    else return Token(NUM_F, idx);
}



Token op(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    if (forward == eol) return Token();
    int state = 0;
    ++forward;
    switch (*(forward - 1)) {
    case '=':
        return Token(EQ, "=");
    case '+':
        return Token(ADD, "+");
    case '-':
        return Token(SUB, "-");
    case '*':
        return Token(MUL, "*");
    case '/':
        return Token(DIV, "/");
    case '%':
        return Token(MOD, "%");
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
            return Token(LE, "<=");
        }
        else return Token(LT, "<");
        break;
    case 2:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(GE, ">=");
        }
        else return Token(GT, ">");
        break;
    case 3:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(NE, "!=");
        }
        else return Token(NOT, "!");
        break;
    case 4:
        if (forward != eol && *forward == '&') {
            ++forward;
            return Token(AND, "&&");
        }
        else {
            --forward;
            return Token();
        }
        break;
    case 5:
        if (forward != eol && *forward == '|') {
            ++forward;
            return Token(OR, "||");
        }
        else {
            --forward;
            return Token();
        }
        break;
    case 6:
        if (forward != eol && *forward == '=') {
            ++forward;
            return Token(ASSIGN, ":=");
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



Token sclit(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    if (forward == eol || (*forward != '\'' && *forward != '\"')) return Token();
    char quotes = *forward;
    ++forward;
    std::string stx;
    bool uke = false;
    while (forward != eol && *forward != quotes) {
        char c = *forward;
        stx += c;
        if (c == '\\') {
            ++forward;
            if (forward == eol) {
                if (quotes == '\'') return Token(UNTER_CHAR);
                else return Token(UNTER_STR);
            }
            if (is_escape(*forward)) uke = true;
            stx += *forward;
        }
        ++forward;
    }
    if (forward == eol) {
        if (quotes == '\'') return Token(UNTER_CHAR);
        else return Token(UNTER_STR);
    }
    ++forward;
    if (quotes == '\'') {
        if (stx.length() == 0) {
            return Token(EMPTY_CHAR);
        }
        else if (stx.length() == 1 || (stx.length() == 2 && stx[0] == '\\')) {
            if (is_escape(stx[1])) return Token(CHAR_LIT, stx);
            else return Token(CHAR_LIT_UKE, stx);
        }
        else {
            return Token(MUL_CHAR);
        }
    }
    if (uke) return Token(STRL_LIT_UKE, stx);
    else return Token(STR_LIT, stx);
}


Token delim(std::string::const_iterator& forward, std::string::const_iterator& eol)
{
    char c = *forward;
    ++forward;
    switch (c) {
    case '(':
        return Token(OPEN_ROUND, "(");
    case ')':
        return Token(CLOSE_ROUND, ")");
    case '{':
        return Token(OPEN_CURLY, "{");
    case '}':
        return Token(CLOSE_CURLY, "}");
    case '[':
        return Token(OPEN_SQUARE, "[");
    case ']':
        return Token(CLOSE_SQUARE, "]");
    case ',':
        return Token(COMMA, ",");
    case ';':
        return Token(SEMICOLON, ";");
    default:
        --forward;
        return Token();
    }
}