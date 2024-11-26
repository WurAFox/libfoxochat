#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include <nlohmann/json.hpp>

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT Payload {
    private:
        const std::string baseUrl = std::string(FOXOGRAM_BASE_API) + std::string(FOXOGRAM_API_VERSION);
        std::string method;
        std::string url;
        ix::WebSocketHttpHeaders headers{{"content-type", "application/json"}};
        nlohmann::json bodyJson;
        std::string body;

    public:
        void addAuth(const std::string &token);

        Payload(std::string method, std::string path, std::string token = "");

        Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                std::string token = "");

        Payload(std::string method, std::string path, std::map<std::string, std::string> headers, nlohmann::json body,
                std::string token = "");

        Payload(std::string method, std::string path, nlohmann::json body, std::string token = "");

        [[nodiscard]] const std::string &getMethod() const;

        [[nodiscard]] const ix::WebSocketHttpHeaders &getHeaders() const;

        [[nodiscard]] const std::string &getBody() const;

        [[nodiscard]] const std::string &getUrl() const;

        [[nodiscard]] const nlohmann::json &getBodyJson() const;
    };

    class FOXOGRAM_LIB_EXPORT HttpClient {
    public:
        static nlohmann::json request(Payload request);
    };
}
