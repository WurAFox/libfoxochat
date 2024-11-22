#pragma once

#include <foxogram/export.h>
#include <ixwebsocket/IXHttpClient.h>
#include <string>
#include <nlohmann/json.hpp>

namespace foxogram {

    class Payload {
    private:
        const std::string baseUrl = "https://api.foxogram.su/v1";
        std::string method;
        std::string url;

        std::string body;
        ix::WebSocketHttpHeaders headers;
    public:
        void addAuth(std::string token);
        Payload(std::string method, std::string path);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                std::unordered_map<std::string, std::string> body);
        Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                std::unordered_map<std::string, std::string> body, std::string token);
        const std::string &getMethod() const;
        const ix::WebSocketHttpHeaders &getHeaders() const;
        const std::string &getBody() const;
        const std::string &getUrl() const;
    };

    class HttpClient {
    public:
        static nlohmann::json request(Payload request);
    };

}