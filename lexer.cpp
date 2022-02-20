#include <iostream>
#include <fstream>
#include <vector>

using CodeLines = std::vector<std::string>;

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



class File : public std::fstream {
    std::string name;
public:
    File(const char* file) : std::fstream{ file }, name{ file } {}
    std::string get_file_name() { return name; }
};

int relop(const std::string& line, int index){
    int state = 0, i = index;
    while(true){
        switch(state){
            case 0:
                if(line[i] == '<') state = 1;
                else if(line[i] == '!') state = 9;
                else if(line[i] == '=') state = 5;
                else if(line[i] == '>') state = 6;
                else return index;
                break;

            case 1:
                i++;
                if(line[i] == '=') state = 2;
                else if(line[i] == '>') state = 3;
                else state = 4;
                break;

            case 2:
                std::cout << "(relop, LE)" << "\n";
                return i;
            
            case 3:
                std::cout << "(relop, NE)" << "\n";
                return i;
            
            case 4:
                i--;
                std::cout << "(relop, LT)" << "\n";
                return i;
            
            case 5:
                std::cout << "(relop, EQ)" << "\n";
                return i;
            
            case 6:
                i++;
                if(line[i] == '=') state = 7;
                else state = 8;
                break;
            
            case 7:
                std::cout << "(relop, GE)" << "\n";
                return i;
            
            case 8:
                i--;
                std::cout << "(relop, GT)" << "\n";
                return i;
            
            case 9:
                i++;
                if(line[i] == '=') state = 3;
                break;
            
            default:
                state = 0;
                break;
        }
    }
}

void tokenizer(const std::string& line){
    int index = 0;
    while(index<line.length()){
        if(line[index] != ' '){
            index = relop(line, index);
            index++;
        }
        else
            index++;
    }
}

void write_back(File& file, CodeLines& cl)
{
    for (const auto& line : cl) {
        std::cout << line << std::endl;
        tokenizer(line);
    }
}

void perform(File& file)
{
    CodeLines cl;
    std::string line;
    while (std::getline(file, line, '\n')) {
        auto found = line.find("//");
        if (found != std::string::npos) {
            line.erase(found);
        }
        line = trim(line);
        if (line.length()) cl.push_back(line);
    }
    write_back(file, cl);
    
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
            perform(file);
        }
    }
    return 0;
}