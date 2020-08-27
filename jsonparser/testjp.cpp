#include "jsonparser.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

auto main() -> int
{
    std::string str;

    std::ifstream inFile;
    inFile.open("./jsonparser/test.json");
    std::stringstream ss;
    ss << inFile.rdbuf();
    str = ss.str();
    inFile.close();

    std::map<std::string, mpaop::jp::JsonToken> parsedData;
    mpaop::jp::JsonParser::parseJsonString(str, parsedData);
    return 0;
}