#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include "nlohmann/json.hpp"

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT Payload {
    private:
        const std::string baseUrl = "https://" + std::string(FOXOGRAM_API);
        std::string method;
        std::string url;
        ix::WebSocketHttpHeaders headers{{"content-type", "application/json"}};
        nlohmann::json bodyJson;
        std::string body;

    public:
        void addAuth(const std::string &token);

        Payload(std::string method, const std::string& path, const std::string& token = "");

        Payload(std::string method, const std::string& path, std::map<std::string, std::string> headers,
                const std::string& token = "");

        Payload(std::string method, const std::string& path, std::map<std::string, std::string> headers, const nlohmann::json& body,
                const std::string& token = "");

        Payload(std::string method, const std::string& path, const nlohmann::json& body, const std::string& token = "");

        [[nodiscard]] const std::string &getMethod() const;

        [[nodiscard]] const ix::WebSocketHttpHeaders &getHeaders() const;

        [[nodiscard]] const std::string &getBody() const;

        [[nodiscard]] const std::string &getUrl() const;

        [[nodiscard]] const nlohmann::json &getBodyJson() const;
    };

    class FOXOGRAM_LIB_EXPORT HttpClient {
    public:
        static nlohmann::json request(const Payload& request);
    };
}
