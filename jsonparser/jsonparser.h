#pragma once

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <queue>

namespace mpaop::jp
{

    union JsonToken
    {
        bool boolean_;
        int64_t number_;
        double double_;
        std::string string_;
        std::map<std::string, JsonToken, std::less<>> object_;
        std::vector<std::map<std::string, JsonToken, std::less<>>> array_;

        JsonToken() : number_(0) {}
        JsonToken(const JsonToken & token) : number_(token.number_) {}
        ~JsonToken() {}
    };

    class JsonParser
    {
    public:
        static bool parseJsonString(const std::string & inStr, std::map<std::string, JsonToken, std::less<>> & parsedData);
        static bool lexString(const std::string & inStr, std::queue<std::string> & lexedData);
        static bool createToken(std::queue<std::string> & lexedData, std::string & outName, JsonToken & outToken);
        static void convertData(std::queue<std::string> & lexedData, std::map<std::string, JsonToken, std::less<>> & parsedData);
    };

}

