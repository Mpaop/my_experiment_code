#include "jsonparser.h"
#include <iostream>
#include <cstring>
#include <charconv>

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
                        num.push_back(* it);
                    }
                    else if (* it == '.')
                    {
                        if (isDouble) return false;
                        else
                        {
                            isDouble = true;
                            id = 'd';
                            num.push_back(* it);
                        }
                    }
                    else
                    {
                        --it;
                        break;
                    }
                    

                    checkIt(++it, inStr);
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

    bool JsonParser::findId(
        std::queue<std::string> & lexedData, 
        std::string & outId
    )
    {
        outId = lexedData.front();
        outId.pop_back();
        lexedData.pop();

        if (strcmp(lexedData.front().c_str(), ":"))
        {
            ThrowWrongFormat false;
        }
        lexedData.pop();

        return true;
    }

    bool JsonParser::findToken(
        std::queue<std::string> & lexedData, 
        JsonToken & outToken
    )
    {
        std::string data = lexedData.front();
        lexedData.pop();
        char id = data.back();
        data.pop_back();

        // s, b, n, i, d, {, [,
        if (id == 's')
        {
            outToken.token_ = data;
        }
        else if (id == 'b')
        {
            outToken.token_ = data[0] == '1' ? true : false;
        }
        else if (id == 'n')
        {
            outToken.token_ = std::nullopt;
        }
        else if (id == 'i')
        {
            int64_t res;
            if(auto [p, e] = std::from_chars(data.data(), data.data() + data.size(), res); e == std::errc())
            {
                outToken.token_ = res;
            }
        }
        else if (id == 'd')
        {
            outToken.token_ = std::atof(data.c_str());
        }
        else if (id == '{')
        {
            outToken.token_ = std::make_shared<JsonToken::object>();
            while(lexedData.front() != "}")
            {
                std::string n;
                JsonToken t;
                if(createToken(lexedData, n, t))
                {
                    auto it = std::get<JsonToken::object_type>(outToken.token_.value())->emplace(n, t);
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
            // in case of an array of json objects
            JsonToken::array_type array = std::make_shared<std::vector<JsonToken>>();
            while (lexedData.front() != "]")
            {
                JsonToken token;
                if(findToken(lexedData, token))
                {
                    array->emplace_back(token);
                }
                else
                {
                    ThrowWrongFormat false;
                }
            }

            outToken.token_ = array;
            lexedData.pop();
        }
        else
        {
            ThrowWrongFormat false;
        }

        return true;
    }

    bool JsonParser::createToken(
        std::queue<std::string> & lexedData,
        std::string & outId,
        JsonToken & outToken
    )
    {
        bool res = false;
        res = findId(lexedData, outId);
        if(! res) return res;
        res = findToken(lexedData, outToken);
        return res;
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

#define INDENT(max) for (int i = 0; i < max; ++i) { std::cout << "\t"; }

    void JsonParser::printBool(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        INDENT(depth)
        std::cout << std::get<bool>(token.token_.value()) << "\n";
    }

    void JsonParser::printInteger(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        INDENT(depth)
        std::cout << std::get<int64_t>(token.token_.value()) << "\n";
    }

    void JsonParser::printDouble(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        INDENT(depth)
        std::cout << std::get<double>(token.token_.value()) << "\n";
    }

    void JsonParser::printString(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        INDENT(depth)
        std::cout << std::get<std::string>(token.token_.value()) << "\n";
    }

    void JsonParser::printObject(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        auto & map = std::get<JsonToken::object_type>(token.token_.value());
        auto d = depth + 1;

        INDENT(d)
        std::cout << "{\n";

        for (auto &e : * map)
        {
            printId(e.first, d);
            printToken(e.second, d);
        }

        INDENT(d)
        std::cout << "}\n";
    }

    void JsonParser::printArray(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        auto & array = std::get<JsonToken::array_type>(token.token_.value());
        auto d = depth + 1;

        INDENT(d)
        std::cout << "[\n";

        for (auto & obj : * array)
        {
            printToken(obj, d);
        }

        INDENT(d)
        std::cout << "]\n";
    }

    void JsonParser::printId(
        const std::string & id, 
        int depth
    )
    {
        INDENT(depth)
        std::cout << id << ": \n";
    }

    void JsonParser::printToken(
        const mpaop::jp::JsonToken & token, 
        int depth
    )
    {
        const int idx = token.token_->index();

        switch(idx)
        {
            case 0:
                printBool(token, depth);
                break;
            case 1:
                printInteger(token, depth);
                break;
            case 2:
                printDouble(token, depth);
                break;
            case 3:
                printString(token, depth);
                break;
            case 4:
                printObject(token, depth);
                break;
            case 5:
                printArray(token, depth);
                break;
            default:
                break;
        }
    }

    void JsonParser::printParsedJsonData(const std::map<std::string, mpaop::jp::JsonToken, std::less<>> & parsedData)
    {
        for(auto & token : parsedData)
        {
            printId(token.first, 0);
            printToken(token.second, 0);
        }
    }
}