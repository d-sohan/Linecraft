#include "utilities.h"
#include "tokens.h"


/**
 * The following function are used for triming whitespaces
 */

const std::string WHITESPACE = " \n\r\t\f\v";
std::string ltrim(const std::string &s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string rtrim(const std::string &s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

std::string trim(const std::string &s) {
    return rtrim(ltrim(s));
}







bool is_escape(char c)
{
    /**
     * checks if the escape character is defined for Linecraft or not
     */
    
    switch (c) {
    case 'n': case 't': case 'r': case 'v': case '\\': case '\'': case '\"':
        return true;
    }
    return false;
}







int check_keywords(const std::string& s)
{
    /**
     * Checks if the string s is present in the keywords vector or not
     * returns the token id computed from the index of the matched keyword in the vector and
     * the symbolic constant defined in include/tokens.h
     */
    
    for (int i = 0, l = keywords.size(); i < l; ++i) {
        if (s == keywords[i]) return KWD_BASE + i + 1;
    }
    return ID;
}