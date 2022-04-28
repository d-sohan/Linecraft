#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "include/utilities.h"
#include "include/tokens.h"
#include "parser.h"




std::vector<std::string> keywords = {
    "int",
    "float",
    "boolean",
    "char",
    "string",
    "return",
    "void",
    "true",
    "false",
    "if",
    "else",
    "while",
};




using dfa_function = std::function < Token(std::string::const_iterator&, std::string::const_iterator&)>;

std::vector<dfa_function> dfas = {
    kid, // keywords and identifier
    op, // operators
    delim, // delimiters
    num, // integer and float literals
    sclit, // string and char literals
};












Token get_next_token(std::string::const_iterator& lexeme_begin, std::string::const_iterator& eol)
{
    std::string::const_iterator forward = lexeme_begin;
    Token tk;
    for (const auto& dfa : dfas) {
        tk = dfa(forward, eol);
        if (tk.token_id == NOT_FOUND) {
            forward = lexeme_begin;
        }
        else break;
    }
    for (lexeme_begin = forward; lexeme_begin != eol && isspace(*lexeme_begin); ++lexeme_begin);
    return tk;
}








void lexer(const std::string& line, int line_number, std::vector<std::pair<Token, int>>& tk_stream)
{
    /**
     * extracts all the tokens from a single codeline and
     * prints their token id, corresponding lexeme and the line number at which it occurs in the original file.
     * Incase of errors like invalid token or other, it prints the respective message along with the line number
     */


    std::string::const_iterator lexeme_begin = line.begin();
    std::string::const_iterator eol = line.end();
    while (lexeme_begin != eol) {
        Token token = get_next_token(lexeme_begin, eol);
        switch (token.token_id) {
        case EMPTY_CHAR: case UNTER_CHAR: case MUL_CHAR: case UNTER_STR: case CHAR_LIT_UKE: case STRL_LIT_UKE:
            break;
        case NOT_FOUND:
            // std::cout << "<Invalid token>";
            do {
                ++lexeme_begin;
            } while (lexeme_begin != eol && *lexeme_begin == ' ');
            break;
        default:
            // std::cout << "<Token " << token.token_id << ", lexeme \"" << token.lexeme << "\">";
            tk_stream.push_back(std::make_pair(token, line_number));
        }
        // std::cout << " @ " << line_number << std::endl;
    }
}










struct CodeLine {
    int line_num;
    std::string code;
    CodeLine(int ln, std::string c) : line_num{ ln }, code{ c }{}
};



void perform(std::vector<CodeLine>& cl)
{
    std::vector<std::pair<Token, int>> tk_stream;
    for (int i = 0, n = cl.size(); i < n; ++i) {
        lexer(cl[i].code, cl[i].line_num, tk_stream);
    }
    std::list<PToken> ptk_stream;
    for (const auto& x : tk_stream) {
        auto token = x.first;
        int line_num = x.second;
        std::string token_str = "";
        switch (token.token_id) {
        case ID:
            token_str = "id";
            break;
        case NUM_I: case NUM_0:
            token_str = "intlit";
            break;
        case NUM_F:
            token_str = "floatlit";
            break;
        case CHAR_LIT:
            token_str = "charlit";
            break;
        case STR_LIT:
            token_str = "strlit";
            break;
        default:
            token_str = token.lexeme;
        }
        ptk_stream.push_back(PToken{ token_str, line_num });
    }
    Parser slr_parser{ "res/pt.csv", "res/cfg.txt" };
    slr_parser.parse(ptk_stream);
}







void load(File& file)
{
    /**
     * This function fetches each line of the file, terminated by a newline, and
     * strips leading and trailing whitespace,
     * removes all comments (which begin with '//') from each of the lines and
     * adds them to a vector
     */


    std::vector<CodeLine> cl;
    std::string line;
    int ln = 0;
    while (std::getline(file, line, '\n')) {
        ++ln;
        auto found = line.find("//");
        if (found != std::string::npos) {
            line.erase(found);
        }
        line = trim(line);
        if (line.length()) cl.push_back(CodeLine(ln, line));
    }
    perform(cl);
}













int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: linecraftc <filename>" << std::endl;
        return 0;
    }
    else {
        File file{ argv[1] };
        if (!file.is_open()) {
            std::cerr << "Couldn't open " << argv[1] << std::endl;
        }
        else {
            load(file);
        }
    }
    return 0;
}