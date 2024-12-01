#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include "nlohmann/json.hpp"

namespace foxogram {
    class LIBFOXOGRAM_EXPORT Payload {
    private:
        const std::string baseUrl = "https://" + std::string(FOXOGRAM_URL) + std::string(FOXOGRAM_API_VERSION);
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

    class LIBFOXOGRAM_EXPORT HttpClient {
    public:
        static nlohmann::json request(const Payload& request);
    };
}
