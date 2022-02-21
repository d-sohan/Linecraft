#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>
#include <fstream>

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

class File : public std::fstream {
    std::string name;
public:
    File(const char* file) : std::fstream{ file }, name{ file } {}
    std::string get_file_name() { return name; }
};

#endif