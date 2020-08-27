#include "jsonparser.h"
#include <iostream>

#define checkIt(it, str) if(it == str.cend()) return false

namespace mpaop::jp
{
    bool JsonParser::parseJsonString(
        const std::string & inStr, 
        std::map<std::string, JsonToken> & parsedData)
    {
        std::queue<std::string> data;
        if(! lexString(inStr, data)) return false;

        convertData(data, parsedData);
        return true;
    }

    bool JsonParser::lexString(
        const std::string & inStr,
        std::queue<std::string> & lexedData
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
                lexedData.push(std::string(1, * it));
            }
            else if(* it == '"')
            {
                std::string t;
                ++it;
                while(* it != '"')
                {
                    t.push_back(* (it++));
                    checkIt(it, inStr);
                }
                if(t.empty()) return false;
                // identifier for string
                t.push_back('s');
                lexedData.push(t);
            }
            else if(* it == 't' || * it == 'f' || * it == 'n')
            {
                std::string comp;
                char id;
                if (* it == 't')
                {
                    comp = "true";
                    id = 'b';
                }
                else if (* it == 'f')
                {
                    comp = "false";
                    id = 'b';
                }
                else
                {
                    comp = "null";
                    id = 'n';
                }

                int64_t sz = comp.size() - 1;
                std::string t;
                for(int32_t i = 0; i < sz; ++i)
                {
                    t.push_back(* (it++));
                    checkIt(it, inStr);
                    if(* it != comp[i + 1]) return false;
                }
                t.push_back(* it);
                t.push_back(id);
                lexedData.push(t);
            }
            else if(* it >= '0' && * it <= '9')
            {
                std::string num;
                char id = 'i';
                bool isDouble = false;
                while(true)
                {
                    if(* it >= '0' && * it <= '9')
                    {
                        num.push_back(* (it++));
                    }
                    else if(* it == '.')
                    {
                        if(isDouble) return false;
                        else
                        {
                            isDouble = true;
                            id = 'd';
                            num.push_back(* (it++));
                        }
                    }
                    else
                    {
                        break;
                    }

                    checkIt(it, inStr);
                }
                num.push_back(id);
                lexedData.push(num);
            }
            else
            {
                return false;   
            }
        }

        return true;
    }

#define ThrowWrongFormat std::cout << "wrong format\n"; return

    void JsonParser::convertData(
        std::queue<std::string> & lexedData, 
        std::map<std::string, JsonToken> & parsedData
    )
    {
        auto sz = lexedData.size();
        if(sz <= 2) return;
        else if(strcmp(lexedData.front().c_str(), "{") || strcmp(lexedData.back().c_str(), "}"))
        {
            ThrowWrongFormat;
        }

        // pop first {
        lexedData.pop();

        while(lexedData.size() > 1)
        {
            // get name of attribute
            std::string name = lexedData.front();
            name.pop_back();
            lexedData.pop();
            
            if(strcmp(lexedData.front().c_str(), ":"))
            {
                ThrowWrongFormat;
            }
            lexedData.pop();

            // create token
            JsonToken token;
            char id = lexedData.front().back();

            

        }
    }
}