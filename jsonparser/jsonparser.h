#pragma once

#include <string>
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
        using object_type = std::shared_ptr<std::map<std::string, JsonToken, std::less<>>>;
        using array_type = std::shared_ptr<std::vector<std::map<std::string, JsonToken, std::less<>>>>;

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
        JsonToken(const JsonToken &token) : token_(token.token_) {}
        ~JsonToken() {}

        [[nodiscard]] auto hasValue() const -> const bool { return this->token_.has_value(); }
    };

    class JsonParser
    {
    public:
        static bool parseJsonString(const std::string &inStr, std::map<std::string, JsonToken, std::less<>> &parsedData);
        static bool lexString(const std::string &inStr, std::queue<std::string> &lexedData);
        static bool createToken(std::queue<std::string> &lexedData, std::string &outName, JsonToken &outToken);
        static void convertData(std::queue<std::string> &lexedData, std::map<std::string, JsonToken, std::less<>> &parsedData);
    };

} // namespace mpaop::jp
