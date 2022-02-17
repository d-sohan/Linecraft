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

void write_back(File& file, CodeLines& cl)
{
    for (const auto& line : cl) {
        std::cout << line << std::endl;
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