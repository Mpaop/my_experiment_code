#include "jsonparser.h"

namespace mpaop::jp
{
    bool JsonParser::parseJsonString(
        const std::string & inStr, 
        std::map<std::string, JsonToken> & parsedData)
    {
        std::vector<std::string> data;
        if(! lexString(inStr, data)) return false;

        convertData(data, parsedData);
        return true;
    }

    bool JsonParser::lexString(
        const std::string & inStr,
        std::vector<std::string> & lexedData
    )
    {
        auto pos = inStr.find_first_of('{');
        if(pos == std::string::npos) return false;

        for(auto it = inStr.cbegin() + pos; it != inStr.cend(); ++it)
        {
            if(* it == EOF) break;
            if(* it == ' ' || * it == ',' || * it == '\n' || * it == '\t') continue;

            if(* it == '{' || * it == '}' || 
               * it == '[' || * it == ']' || 
               * it == ':')
            {
                lexedData.push_back(std::string(1, * it));
            }
            else if(* it == '"')
            {
                std::string t;
                t.push_back(* (it++));
                while(* (it++) != '"') t.push_back(* it);
                t.push_back(* it);
                if(t.size() == 2) return false;
                lexedData.push_back(t);
            }
            else if(* it == 't')
            {

            }
            else if(* it == 'f')
            {

            }
            else if(* it == 'n')
            {

            }
            else if(* it >= '0' && * it <= '9')
            {

            }
            else
            {
                return false;   
            }
        }

        return true;
    }

    void JsonParser::convertData(
        std::vector<std::string> & lexedData, 
        std::map<std::string, JsonToken> & parsedData
    )
    {

    }
}