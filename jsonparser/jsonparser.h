#pragma once

#include<string>
#include<map>
#include<vector>

namespace mpaop::jp
{

    union JsonToken
    {
        bool boolean_;
        int64_t number_;
        double double_;
        std::string string_;
        std::map<std::string, JsonToken> object_;
        std::vector<std::map<std::string, JsonToken>> array_;

        JsonToken() : number_(0) {}
        ~JsonToken() {}
    };

    class JsonParser
    {
    public:
        static bool parseJsonString(const std::string & inStr, std::map<std::string, JsonToken> & parsedData);
        static bool lexString(const std::string & inStr, std::vector<std::string> & lexedData);
        static void convertData(std::vector<std::string> & lexedData, std::map<std::string, JsonToken> & parsedData);

    };

}

