#pragma once

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <queue>
#include <memory>
#include <variant>
#include <optional>

namespace mpaop::jp
{

    struct JsonToken
    {
        using object = std::map<std::string, JsonToken, std::less<>>;
        using object_type = std::shared_ptr<object>;
        using array_type = std::shared_ptr<std::vector<JsonToken>>;

        std::optional<
            std::variant<
                bool,
                int64_t,
                double,
                std::string,
                object_type,
                array_type
                >
            > token_;

        JsonToken() : token_() {}
        JsonToken(const JsonToken & token) : token_(token.token_) {}
        ~JsonToken() {}

        [[nodiscard]] auto hasValue() const -> const bool { return this->token_.has_value(); }
    };

    class JsonParser
    {
    private:
        static bool findId(std::queue<std::string> & lexedData, std::string & outId);
        static bool findToken(std::queue<std::string> & lexedData, JsonToken & outToken);

        template<typename T>
        static void printValue(const mpaop::jp::JsonToken & token, int depth);

        static void printId(const std::string & id, int depth);
        static void printToken(const mpaop::jp::JsonToken & token, int depth);

    public:
        static bool parseJsonString(const std::string & inStr, std::map<std::string, JsonToken, std::less<>> & parsedData);
        static bool lexString(const std::string & inStr, std::queue<std::string> & lexedData);
        static bool createToken(std::queue<std::string> & lexedData, std::string & outId, JsonToken & outToken);
        static void convertData(std::queue<std::string> & lexedData, std::map<std::string, JsonToken, std::less<>> & parsedData);

        static void printParsedJsonData(const std::map<std::string, mpaop::jp::JsonToken, std::less<>> & parsedData);
    };

} // namespace mpaop::jp
