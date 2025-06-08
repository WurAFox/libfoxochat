#pragma once

#include <foxochat/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include "nlohmann/json.hpp"

#ifndef FOXOCHAT_BASE_API_URL
#ifdef IXWEBSOCKET_USE_TLS
#define FOXOCHAT_BASE_API_URL "https://api" FOXOCHAT_URL
#else
#define FOXOCHAT_BASE_API_URL "http://api" FOXOCHAT_URL
#endif
#endif

namespace foxochat {
    class LIBFOXOCHAT_EXPORT Payload {
    private:
        inline static std::string baseUrl = FOXOCHAT_BASE_API_URL;
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

    class LIBFOXOCHAT_EXPORT HttpClient {
    public:
        static nlohmann::json request(const Payload& request);
    };
}
