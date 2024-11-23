#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>

#include <utility>

foxogram::Me::Me(std::string token) {
    this->token = std::move(token);
    auto u = this->fetchMe();
    this->id = u.getId();
    this->username = u.getUsername();
    this->avatar = u.getAvatar();
    this->flags = u.getFlags();
    this->type = u.getType();
    this->createdAt = u.getCreatedAt();
}

foxogram::Me::Me(std::string username, std::string email, std::string password) {
    this->token = Me::signup(username, email, password);
    auto u = this->fetchMe();
    this->id = u.getId();
    this->username = u.getUsername();
    this->avatar = u.getAvatar();
    this->flags = u.getFlags();
    this->type = u.getType();
    this->createdAt = u.getCreatedAt();
}

foxogram::Me::Me(std::string email, std::string password) {
    this->token = Me::login(email, password);
    auto u = this->fetchMe();
    this->id = u.getId();
    this->username = u.getUsername();
    this->avatar = u.getAvatar();
    this->flags = u.getFlags();
    this->type = u.getType();
    this->createdAt = u.getCreatedAt();
}

foxogram::User foxogram::Me::fetchUser(long long id) {
    auto j = foxogram::HttpClient::request(Payload("GET", std::string("/users/")+std::to_string(id), this->token));
    return foxogram::User(0, std::basic_string(""), std::basic_string(""), 0, 0, 0);
}

std::string foxogram::Me::login(std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/signup", std::unordered_map<std::string, std::string>{
        {"email", email},
        {"password", password}
    }));
    return std::string(j.at("access_token"));
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/login",
                                                             std::unordered_map<std::string, std::string>{
        {"username", username},
        {"email", email},
        {"password", password}
    }));
    return std::string(j.at("access_token"));
}

bool foxogram::Me::verifyEmail(std::string code) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/v1/auth/email/verify/"+code, token));
    return j.at("ok") == "true";
}

void foxogram::Me::deleteUser(std::string password) {
    foxogram::HttpClient::request(Payload("POST", "/auth/delete",
                                          std::unordered_map<std::string, std::string>{
        {"password", password}
        }, token));
}

void foxogram::Me::confirmDeleteUser(std::string code) {
    foxogram::HttpClient::request(Payload("POST", "/auth/delete/confirm"+code, token));
}

foxogram::User foxogram::Me::fetchMe() {
    auto j = foxogram::HttpClient::request(Payload("GET", "/users/@me", token));
    return foxogram::User(0, std::basic_string(""), std::basic_string(""), 0, 0, 0);
}
