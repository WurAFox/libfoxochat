#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>

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
    return foxogram::User(
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(),j.at("flags").get<long long>(),
            j.at("type"), j.at("createdAt").get<long long>());
}

std::string foxogram::Me::login(std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/signup", std::unordered_map<std::string, std::string>{
        {"email", email},
        {"password", password}
    }));
    return j.at("access_token").get<std::string>();
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/login",
                                                             std::unordered_map<std::string, std::string>{
        {"username", username},
        {"email", email},
        {"password", password}
    }));
    return j.at("access_token").get<std::string>();
}

bool foxogram::Me::verifyEmail(std::string code) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/v1/auth/email/verify/"+code, token));
    return j.at("ok").get<bool>();
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
    return foxogram::User(
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(),j.at("flags").get<long long>(),
            j.at("type"), j.at("createdAt").get<long long>());
}
