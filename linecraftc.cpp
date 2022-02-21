#include <iostream>
#include <fstream>
#include <vector>
#include <functional>
#include "include/utilities.h"
#include "tokens.h"

struct CodeLine {
    int line_num;
    std::string code;
    CodeLine(int ln, std::string c) : line_num{ ln }, code{ c }{}
};




using dfa_function = std::function < Token(std::string::const_iterator&, std::string::const_iterator&)>;



std::vector<dfa_function> dfas = {
    keywords,
    iden,
    op,
    delim,
    num,
    // add function names here in order
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






void lexer(const std::string& line, int line_number) {
    std::string::const_iterator lexeme_begin = line.begin();
    std::string::const_iterator eol = line.end();
    while (lexeme_begin != eol) {
        Token token = get_next_token(lexeme_begin, eol);
        if (token.token_id == NOT_FOUND) {
            std::cout << "<Invalid token>";
            do {
                ++lexeme_begin;
            } while (lexeme_begin != eol && *lexeme_begin == ' ');
        }
        else {
            std::cout << "<Token " << token.token_id << ", lexeme \"" << token.lexeme << "\">";
        }
        std::cout << " @ " << line_number << std::endl;
    }
}







void perform(std::vector<CodeLine>& cl)
{
    for (int i = 0, n = cl.size(); i < n; ++i) {
        // std::cout << cl[i].code << std::endl;
        lexer(cl[i].code, cl[i].line_num);
    }
}







void load(File& file)
{
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
    if (argc == 1) {
        std::cout << "No file provided\n";
        return 0;
    }
    for (int i = 1; i < argc; ++i) {
        File file{ argv[i] };
        if (!file.is_open()) {
            std::cerr << "Couldn't open " << argv[i] << std::endl;
        }
        else {
            load(file);
        }
    }
    return 0;
}