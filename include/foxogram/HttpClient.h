#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include <nlohmann/json.hpp>

namespace foxogram {

    class FOXOGRAM_LIB_EXPORT Payload {
    private:
        const std::string baseUrl = "http://api.foxogram.su/v1";
        std::string method;
        std::string url;
        ix::WebSocketHttpHeaders headers {{"content-type", "application/json"}};
        nlohmann::json bodyJson;
        std::string body;

    public:
        void addAuth(std::string token);
        Payload(std::string method, std::string path);
        Payload(std::string method, std::string path, std::string token);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers, std::string token);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers, nlohmann::json body);
        Payload(std::string method, std::string path, nlohmann::json body);
        Payload(std::string method, std::string path, nlohmann::json body, std::string token);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers, nlohmann::json body, std::string token);
        const std::string &getMethod() const;
        const ix::WebSocketHttpHeaders &getHeaders() const;
        const std::string &getBody() const;
        const std::string &getUrl() const;
        const nlohmann::json &getBodyJson() const;
    };

    class FOXOGRAM_LIB_EXPORT HttpClient {
    public:
        static nlohmann::json request(Payload request);
    };

}