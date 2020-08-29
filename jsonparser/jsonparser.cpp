#include "jsonparser.h"
#include <iostream>
#include <cstring>

#define checkIt(it, str) if (it == str.cend()) return false

namespace mpaop::jp
{
    bool JsonParser::parseJsonString(
        const std::string & inStr,
        std::map<std::string, JsonToken, std::less<>> & parsedData)
    {
        std::queue<std::string> data;
        if (!lexString(inStr, data))
            return false;

        convertData(data, parsedData);
        return true;
    }

    bool JsonParser::lexString(
        const std::string & inStr,
        std::queue<std::string> & lexedData)
    {
        auto pos = inStr.find_first_of('{');
        if (pos == std::string::npos)
            return false;

        for (auto it = inStr.cbegin() + pos; it != inStr.cend(); ++it)
        {
            if (* it == EOF)
                break;
            if (* it == ' ' || * it == ',' || * it == '\n' || * it == '\t')
                continue;

            if (* it == '{' || * it == '}' ||
                * it == '[' || * it == ']' ||
                * it == ':')
            {
                lexedData.push(std::string(1, * it));
            }
            else if (* it == '"')
            {
                std::string t;
                ++it;
                while (* it != '"')
                {
                    t.push_back(* (it++));
                    checkIt(it, inStr);
                }
                if (t.empty())
                    return false;
                // identifier for string
                t.push_back('s');
                lexedData.push(t);
            }
            else if (* it == 't' || * it == 'f' || * it == 'n')
            {
                std::string comp;
                std::string ins;
                if (* it == 't')
                {
                    comp = "true";
                    ins = "1b";
                }
                else if (* it == 'f')
                {
                    comp = "false";
                    ins = "0b";
                }
                else
                {
                    comp = "null";
                    ins = "n";
                }

                int64_t sz = comp.size() - 1;
                for (int32_t i = 0; i < sz; ++i)
                {
                    checkIt(++it, inStr);
                    if (* it != comp[i + 1]) return false;
                }
                lexedData.push(ins);
            }
            else if (* it >= '0' && * it <= '9')
            {
                std::string num;
                char id = 'i';
                bool isDouble = false;
                while (true)
                {
                    if (* it >= '0' && * it <= '9')
                    {
                        num.push_back(* (it++));
                    }
                    else if (* it == '.')
                    {
                        if (isDouble) return false;
                        else
                        {
                            isDouble = true;
                            id = 'd';
                            num.push_back(*(it++));
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

    bool JsonParser::createToken(
        std::queue<std::string> & lexedData,
        std::string & outName,
        JsonToken & outToken
    )
    {
        outName = lexedData.front();
        outName.pop_back();
        lexedData.pop();

        if (strcmp(lexedData.front().c_str(), ":"))
        {
            ThrowWrongFormat false;
        }
        lexedData.pop();

        std::string data = lexedData.front();
        lexedData.pop();
        char id = data.back();

        // s, b, n, i, d, {, [,
        if (id == 's')
        {
            outToken.string_ = data;
        }
        else if (id == 'b')
        {
            outToken.boolean_ = data[0] == '1' ? true : false;
        }
        else if (id == 'n')
        {
            outToken.isnull_ = true;
        }
        else if (id == 'i')
        {
            outToken.number_ = std::atoi(data.c_str());
        }
        else if (id == 'd')
        {
            outToken.double_ = std::atof(data.c_str());
        }
        else if (id == '{')
        {
            outToken.object_ = std::map<std::string, JsonToken, std::less<>>();
            while(lexedData.front() != "}")
            {
                std::string n;
                JsonToken t;
                if(createToken(lexedData, n, t))
                {
                    auto it = outToken.object_.emplace(n, t);
                    if(! it.second)
                    {
                        ThrowWrongFormat false;
                    }
                }   
                else
                {
                    ThrowWrongFormat false;
                }
            }
            lexedData.pop();
        }
        else if (id == '[')
        {
            outToken.array_ = std::vector<std::map<std::string, JsonToken, std::less<>>>();
            while(lexedData.front() != "]")
            {
                if(strcmp(lexedData.front().c_str(), "{"))
                {
                    ThrowWrongFormat false;
                }
                lexedData.pop();

                std::map<std::string, JsonToken, std::less<>> map;
                while(lexedData.front() != "}")
                {
                    std::string n;
                    JsonToken t;
                    if(createToken(lexedData, n, t))
                    {
                        auto it = map.emplace(n, t);
                        if(! it.second)
                        {
                            ThrowWrongFormat false;
                        }
                    }
                    else
                    {
                        ThrowWrongFormat false;
                    }
                    
                    outToken.array_.emplace_back(map);
                }
                lexedData.pop();
            }
            lexedData.pop();
        }
        else
        {
            ThrowWrongFormat false;
        }

        return true;
    }

    void JsonParser::convertData(
        std::queue<std::string> & lexedData,
        std::map<std::string, JsonToken, std::less<>> & parsedData)
    {
        auto sz = lexedData.size();
        if (sz <= 2)
            return;
        else if (strcmp(lexedData.front().c_str(), "{") || strcmp(lexedData.back().c_str(), "}"))
        {
            ThrowWrongFormat;
        }

        // pop first {
        lexedData.pop();

        while (lexedData.size() > 1)
        {
            std::string name;
            JsonToken token;

            if(createToken(lexedData, name, token))
            {
                auto it = parsedData.emplace(name, token);
                if(! it.second)
                {
                    ThrowWrongFormat;
                }
            }
            else
            {
                ThrowWrongFormat;
            }
        }
    }
}