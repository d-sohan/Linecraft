#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stack>
#include <list>
#include <iomanip>
#include "include/tokens.h"





void get_content(std::vector<std::vector<std::string>>& content, std::string parse_table_file);


class Parse_table {
    std::unordered_map<std::string, std::vector<std::string>> action_table;
    std::unordered_map<std::string, std::vector<int>> goto_table;
    std::vector<std::string> action_headers;
    std::vector<std::string> goto_headers;
    int state_cnt;
public:
    Parse_table(std::string parse_table_file);
    std::string get_action(std::string terminal, int state) {
        if (std::find(action_headers.begin(), action_headers.end(), terminal) != action_headers.end())
            return action_table[terminal].at(state);
        else return "u";
    }
    int get_gotostate(std::string nonterminal, int prev_state) {
        return goto_table[nonterminal].at(prev_state);
    }

    int get_state_cnt() { return state_cnt; }
    const auto& get_action_headers() { return action_headers; }
    const auto& get_goto_headers() { return goto_headers; }
};


struct PToken {
    std::string token_str;
    int line_num;
    PToken(std::string tk, int ln) : token_str{ tk }, line_num{ ln } {}
    PToken(std::string s) : token_str{ s }, line_num{ -1 } {}
};

class Reducer {
    std::vector<std::pair<std::string, int>> reducer;
public:
    Reducer(std::string cfg_file);
    int get_reduce_by(int n) { return reducer.at(n).second * 2; }
    std::string get_reduce_to(int n) { return reducer.at(n).first; }
};

// class Printer {
//     std::streamsize width = std::cout.width();
// public:
//     Printer(int w) {
//         std::cout.width(w);
//     }
//     void configure(std::string output) {
//         if (output == "stack") {
//             std::cout << std::left;
//         }
//         else if (output == "buffer") {
//             std::cout << std::right;
//         }
//     }
//     ~Printer() {
//         std::cout.width(width);
//     }
// };

class Parser {
    Parse_table slr_pt;
    Reducer red;
public:
    Parser(std::string parse_table_file, std::string cfg_file) : slr_pt{ parse_table_file }, red{ cfg_file } {}
    void parse(std::list<PToken>& ptk_stream);
    void pmr(std::vector<std::string>& stack, std::list<PToken>& ptk_stream, std::list<PToken>::iterator& it);
};
