#include <utility>

#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <ixwebsocket/IXNetSystem.h>

bool isWSAInitialized = false;

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

    Payload::Payload(std::string method, std::string path, std::string token) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->addAuth(std::move(token));
    }

    Payload::Payload(std::string method, std::string path,
                     std::map<std::string, std::string> headers, std::string token) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->headers.merge(headers);
        this->addAuth(std::move(token));
    }

    Payload::Payload(std::string method, std::string path,
                     nlohmann::json body, std::string token) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        std::string strBody;
        for (const auto& pair : body) {
            strBody += to_string(pair.at(1)) + "=" + to_string(pair.at(2)) + "+";
        }
        this->body = strBody.substr(0, strBody.size()-1);
        this->bodyJson = body;
        this->addAuth(std::move(token));
    }

    Payload::Payload(std::string method, std::string path, std::map<std::string, std::string> headers,
                     nlohmann::json body, std::string token) {
        this->method = std::move(method);
        this->url = this->baseUrl + path;
        this->headers.merge(headers);
        std::string strBody;
        for (const auto& pair : body) {
            strBody += to_string(pair.at(1)) + "=" + to_string(pair.at(2)) + "+";
        }
        this->body = strBody.substr(0, strBody.size()-1);
        this->bodyJson = body;
        this->addAuth(std::move(token));
    }

    const nlohmann::json &Payload::getBodyJson() const {
        return bodyJson;
    }

    nlohmann::json HttpClient::request(Payload payload) {
        if (!isWSAInitialized) {
            ix::initNetSystem();
            isWSAInitialized = true;
        }
        ix::HttpClient httpClient;
        ix::HttpRequestArgsPtr args = httpClient.createRequest();
        args->extraHeaders = payload.getHeaders();
        ix::HttpResponsePtr r;
        if (payload.getMethod() == "GET") {
            r = httpClient.get(payload.getUrl()+"?"+payload.getBody(), args);
        } else if (payload.getMethod() == "POST") {
            r = httpClient.post(payload.getUrl(), to_string(payload.getBodyJson()), args);
        } else if (payload.getMethod() == "PUT") {
            r = httpClient.put(payload.getUrl(), to_string(payload.getBodyJson()), args);
        } else if (payload.getMethod() == "DELETE") {
            r = httpClient.Delete(payload.getUrl(), args);
        } else if (payload.getMethod() == "PATCH") {
            r = httpClient.patch(payload.getUrl(), to_string(payload.getBodyJson()), args);
        } else {
            throw std::exception("Invalid method");
        }
        if (!r->errorMsg.empty()) {
            throw HttpException(r->errorMsg);
        }

        nlohmann::json j = nlohmann::json::parse(r->body);
        if (r->statusCode != 200) {
            switch (j.at("code").get<int>()) {
                case(101): throw MessageNotFoundException();
                case(201): throw ChannelNotFoundException();
                case(301): throw UserUnauthorizatedException();
                case(302): throw UserEmailNotVerfiedException();
                case(304): throw UserWithThisEmailAlreadyExistException();
                case(305): throw UserCredentialsIsInvalidException();
                case(401): throw MemberInChannelNotFoundException();
                case(402): throw MemberAlreadyInChannelException();
                case(403): throw MissingPermissionException();
                case(501): throw CodeIsInvalidException();
                case(503): throw CodeExpiredException();
                default: throw HttpException(j.at("message").get<std::string>());
            }
        }

        return j;
    }

}