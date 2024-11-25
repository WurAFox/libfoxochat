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
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/login", nlohmann::json({
        {"email", email},
        {"password", password}
    })));
    return j.at("accessToken").get<std::string>();
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/signup",
                                                             nlohmann::json({
        {"username", username},
        {"email", email},
        {"password", password}
    })));
    return j.at("accessToken").get<std::string>();
}

bool foxogram::Me::verifyEmail(std::string code) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/v1/auth/email/verify/"+code, token));
    return j.at("ok").get<bool>();
}

void foxogram::Me::deleteUser(std::string password) {
    foxogram::HttpClient::request(Payload("POST", "/auth/delete",
                                          nlohmann::json({
        {"password", password}
        }), token));
}

bool foxogram::Me::confirmDeleteUser(std::string code) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/auth/delete/confirm"+code, token));
    return j.at("ok").get<bool>();
}

foxogram::User foxogram::Me::fetchMe() {
    auto j = foxogram::HttpClient::request(Payload("GET", "/users/@me", token));
    return foxogram::User(
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(),j.at("flags").get<long long>(),
            j.at("type"), j.at("createdAt").get<long long>());
}

foxogram::Channel foxogram::Me::createChannel(std::string name, int type) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/create",
                                                   nlohmann::json({{"name", name}, {"type", type}}), token));
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
            j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::joinChannel(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/"+std::to_string(id)+"/join", token));
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                                     j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::fetchChannel(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("GET", "/channels/"+std::to_string(id), token));
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                                     j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Message foxogram::Me::fetchMessage(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("GET", "/messages/"+std::to_string(id), token));
    auto message = foxogram::Message(j.at("id").get<long long>(), nullptr, j.at("authorId").get<long long>(),
            j.at("timestamp").get<long long>(), j.at("attachments").get<std::list<std::string>>());
    message.token = token;
    return message;
}
