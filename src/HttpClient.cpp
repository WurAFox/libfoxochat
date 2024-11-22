#include <utility>

#include "foxogram/HttpClient.h"

namespace foxogram {

    const std::string &Payload::getMethod() const {
        return method;
    }

    const ix::WebSocketHttpHeaders &Payload::getHeaders() const {
        return headers;
    }

    const std::string &Payload::getBody() const {
        return body;
    }

    const std::string &Payload::getUrl() const {
        return url;
    }

    void Payload::addAuth(std::string token) {
        this->headers.insert({{"Authorization", "Bearer " + token}});
    }

    Payload::Payload(std::string method, std::string path) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
    }

    Payload::Payload(std::string method, std::string path,
                     std::map<std::string, std::string> headers) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->headers.merge(headers);
    }

    Payload::Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                     std::unordered_map<std::string, std::string> body) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->headers.merge(headers);
        std::string strBody = "";
        for (const auto& pair : body) {
            strBody += pair.first + "+" + pair.second;
        }
        this->body = strBody;
    }

    Payload::Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                     std::unordered_map<std::string, std::string> body, std::string token) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->headers.merge(headers);
        std::string strBody = "?";
        for (const auto& pair : body) {
            strBody += pair.first + "+" + pair.second;
        }
        this->body = strBody;
        this->addAuth(std::move(token));
    }

    nlohmann::json HttpClient::request(Payload payload) {
        ix::HttpClient httpClient;
        ix::HttpRequestArgsPtr args = httpClient.createRequest();
        args->extraHeaders = payload.getHeaders();
        ix::HttpResponsePtr r;

        if (payload.getMethod() == "GET") {
            r = httpClient.get(payload.getUrl()+"?"+payload.getBody(), args);
        } else if (payload.getMethod() == "POST") {
            r = httpClient.post(payload.getUrl(), payload.getBody(), args);
        } else if (payload.getMethod() == "PUT") {
            r = httpClient.put(payload.getUrl(), payload.getBody(), args);
        } else if (payload.getMethod() == "DELETE") {
            r = httpClient.Delete(payload.getUrl(), args);
        } else if (payload.getMethod() == "PATCH") {
            r = httpClient.patch(payload.getUrl(), payload.getBody(), args);
        }

        return nlohmann::json::parse(r->body);
    }

}