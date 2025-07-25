#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r\f\v");
    if (std::string::npos == first)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(first, (last - first + 1));
}
std::vector<std::string> split2(std::string s, char delimiter) {
    std::vector<std::string> substrings;
    if (s == "") {
        return substrings; // Return an empty vector if input is null
    }

    std::string str(s); // Convert char* to std::string for easier manipulation
    std::istringstream iss(str);
    std::string segment;

    while (std::getline(iss, segment, delimiter)) {
        substrings.push_back(segment);
    }

    return substrings;
}

std::string read2(const char name[]){
    std::ifstream File(name);
    std::string text;
    std::string all;
    while (getline (File, text))
        all += text + "\n";
    File.close();
    return all;
}
void write2(char name[], std::string data){
    std::ofstream File(name);
    File << data;
    File.close();
}
void write2_c(const char name[], std::string data){
    std::ofstream File(name);
    File << data;
    File.close();
}