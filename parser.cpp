#include "parser.h"

void get_content(std::vector<std::vector<std::string>>& content, std::string parse_table_file) {
    std::vector<std::string> row;
    std::fstream ifs(parse_table_file, std::ios::in);
    std::string line, word;
    if (ifs.is_open()) {
        while (getline(ifs, line)) {
            row.clear();

            std::stringstream str(line);

            while (getline(str, word, ',')) {
                if (word == "comma") word = ",";
                row.push_back(word);
            }
            content.push_back(row);
        }
    }
    else {
        std::cerr << "couldn't open file" << std::endl;
    }
}


Parse_table::Parse_table(std::string parse_table_file) {
    std::vector<std::vector<std::string>> content;
    get_content(content, parse_table_file);
    std::vector<std::string> header = content[0];
    state_cnt = content.size() - 1;
    int dollar_index = std::find(header.begin(), header.end(), "$") - header.begin();
    for (int i = 1; i <= dollar_index; ++i) {
        action_headers.push_back(header[i]);
        std::vector<std::string> actions;
        for (int r = 1; r < content.size(); ++r) {
            if (isalpha(content[r][i][0]))
                actions.push_back(content[r][i]);
            else
                actions.push_back("pmr");
        }
        action_table[header[i]] = actions;
    }
    for (int i = dollar_index + 1; i < header.size(); ++i) {
        goto_headers.push_back(header[i]);
        std::vector<int> gotos;
        for (int r = 1; r < content.size(); ++r) {
            if (isdigit(content[r][i][0]))
                gotos.push_back(std::stoi(content[r][i]));
            else
                gotos.push_back(-1);
        }
        std::string nt = header[i];
        std::transform(nt.begin(), nt.end(), nt.begin(), ::toupper);
        goto_table[nt] = gotos;
    }
}

Reducer::Reducer(std::string cfg_file) {
    std::fstream ifs(cfg_file, std::ios::in);
    if (ifs.is_open()) {
        std::string line;
        while (getline(ifs, line)) {
            std::stringstream str(line);
            std::string nt, w;
            str >> nt;
            str >> w;
            int cnt = 0;
            while (str >> w) {
                if (w != "''") ++cnt;
            }
            reducer.push_back({ nt, cnt });
        }
    }
}



void print_stack(std::vector<std::string>& stack) {
    std::string output = "Stack:[";
    for (const auto& x : stack) {
        output += x;
        output += ",";
    }
    output += "]";
    std::cout << output << std::endl;
}

void print_buffer(std::list<PToken>& ptks, std::list<PToken>::iterator& itr) {
    std::string output = "Input:[";
    for (auto it = itr; it != ptks.end(); it++) {
        output += it->token_str;
        output += ",";
    }
    output += "]";
    std::cout << output << std::endl;
}

void print_trace(std::vector<std::string>& stack, std::list<PToken>& ptks, std::list<PToken>::iterator itr) {
    // Printer printer(100);
    // printer.configure("stack");
    print_stack(stack);
    // printer.configure("buffer");
    print_buffer(ptks, itr);
    // std::cout << std::endl;
}

void Parser::pmr(std::vector<std::string>& stack,
    std::list<PToken>& ptk_stream,
    std::list<PToken>::iterator& it) {
    int i = 0;
    for (i = stack.size() - 1; i > 0; --i) {
        if (isdigit(stack.at(i).at(0))) {
            int ps = std::stoi(stack.at(i));
            if (slr_pt.get_gotostate("STMT-LIST", ps) != -1) {
                break;
            }
        }
    }
    for (int j = stack.size() - 1; j > i; --j) {
        stack.pop_back();
    }
    // while (stack.back() != "0" && (!isdigit(stack.back()[0]) || slr_pt.get_gotostate("STMT-LIST", std::stoi(stack.back())) == -1)) {
    //     std::cout << stack.back() << std::stoi(stack.back()) << std::endl;
    //     stack.pop_back();
    // }
    if (stack.back() == "0") {
        stack.push_back("PROGRAM");
        while (it->token_str != "$") ++it;
    }
    else {
        stack.push_back("STMT-LIST");
        while (it->token_str != "}") ++it;
    }
}

void Parser::parse(std::list<PToken>& ptk_stream) {
    ptk_stream.push_back(PToken("$"));
    std::vector<std::string> stack;
    std::vector<PToken> errors;
    stack.push_back("0");
    auto it = ptk_stream.begin();
    while (it != ptk_stream.end()) {
        print_trace(stack, ptk_stream, it);
        if (isdigit(stack.back()[0])) {
            std::string action = slr_pt.get_action(it->token_str, std::stoi(stack.back()));
            switch (action[0]) {
            case 's':
                stack.push_back(it->token_str);
                stack.push_back(action.substr(1));
                ++it;
                break;
            case 'r':
            {
                for (int j = 0, n = red.get_reduce_by(std::stoi(action.substr(1))); j < n; ++j) {
                    stack.pop_back();
                }
                stack.push_back(red.get_reduce_to(std::stoi(action.substr(1))));
                break;
            }
            case 'a':
                ++it;
                break;
            case 'u':
                errors.push_back(*it);
                it = ptk_stream.end();
                break;
            case 'e':
                it = ptk_stream.end();
                break;
            case 'p':
                errors.push_back(*it);
                pmr(stack, ptk_stream, it);
            }
        }
        else {
            int prev_state = std::stoi(stack[stack.size() - 2]);
            stack.push_back(std::to_string(slr_pt.get_gotostate(stack.back(), prev_state)));
        }
    }
    if (errors.size() > 0) {
        std::cout << std::endl;
        std::cout << "<<-----------------------------------ERRORS----------------------------------->>" << std::endl;
        for (const auto& x : errors) {
            std::cout << x.token_str << std::endl;
        }
    }
}

// int main() {
//     std::list<PToken> ptk_stream;
//     std::string line;
//     while (getline(std::cin, line)) {
//         std::stringstream str(line);
//         std::string w;
//         while (str >> w) {
//             ptk_stream.push_back(PToken(w));
//         }
//     }
//     Parser parser{ "pt.csv", "cfg.txt" };
//     parser.parse(ptk_stream);
//     return 0;
// }