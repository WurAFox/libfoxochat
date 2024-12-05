#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include "nlohmann/json.hpp"

#ifndef FOXOGRAM_BASE_API_URL
#define FOXOGRAM_BASE_API_URL "https://api." FOXOGRAM_URL FOXOGRAM_API_VERSION
#endif

namespace foxogram {
    class LIBFOXOGRAM_EXPORT Payload {
    private:
        inline static std::string baseUrl = FOXOGRAM_BASE_API_URL;
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
