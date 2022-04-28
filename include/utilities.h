#pragma once

#include <string>
#include <fstream>
#include <array>
#include <vector>
#include <sstream>
#include <iostream>

std::string ltrim(const std::string &s);
std::string rtrim(const std::string &s);
std::string trim(const std::string &s);

class File : public std::fstream {
    std::string name;
public:
    File(const char* file) : std::fstream{ file }, name{ file } {}
    std::string get_file_name() { return name; }
};

bool is_escape(char c);

int check_keywords(const std::string& s);


