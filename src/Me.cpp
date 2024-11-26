#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>

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
    this->token = Me::signup(std::move(username), std::move(email), std::move(password));
    auto u = this->fetchMe();
    this->id = u.getId();
    this->username = u.getUsername();
    this->avatar = u.getAvatar();
    this->flags = u.getFlags();
    this->type = u.getType();
    this->createdAt = u.getCreatedAt();
}

foxogram::Me::Me(std::string email, std::string password) {
    this->token = Me::login(std::move(email), std::move(password));
    auto u = this->fetchMe();
    this->id = u.getId();
    this->username = u.getUsername();
    this->avatar = u.getAvatar();
    this->flags = u.getFlags();
    this->type = u.getType();
    this->createdAt = u.getCreatedAt();
}

foxogram::User foxogram::Me::fetchUser(long long id) {
    auto j = foxogram::HttpClient::request(Payload("GET",
                                                   std::string("/users/")+std::to_string(id), this->token));
    if (j.at("ok").is_boolean()) {
        if (!j.at("ok").get<bool>()) {
            switch (j.at("code").get<int>()) {
                case(301): throw foxogram::UserUnauthorizatedException();
                case(302): throw foxogram::UserEmailNotVerfiedException();
                default: throw HttpException(j.at("message").get<std::string>());
            }
        }
    }
    return {
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(),j.at("flags").get<long long>(),
            j.at("type"), j.at("createdAt").get<long long>()};
}

std::string foxogram::Me::login(std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/login", nlohmann::json({
        {"email", email},
        {"password", password}
    })));
    if (j.at("ok").is_boolean()) {
        if (!j.at("ok").get<bool>()) {
            switch (j.at("code").get<int>()) {
                case(305): throw foxogram::UserCredentialsIsInvalidException();
                default: throw HttpException(j.at("message").get<std::string>());
            }
        }
    }
    return j.at("accessToken").get<std::string>();
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/auth/signup", nlohmann::json({
        {"username", username},
        {"email", email},
        {"password", password}
    })));
    if (j.at("ok").is_boolean()) {
        if (!j.at("ok").get<bool>()) {
            switch (j.at("code").get<int>()) {
                case(304): throw foxogram::UserWithThisEmailAlreadyExistException();
                default: throw HttpException(j.at("message").get<std::string>());
            }
        }
    }
    return j.at("accessToken").get<std::string>();
}

bool foxogram::Me::verifyEmail(std::string code) {
    nlohmann::json j = foxogram::HttpClient::request(Payload("POST", "/v1/auth/email/verify/"+code, token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(501): throw foxogram::CodeIsInvalidException();
            case(503): throw foxogram::CodeExpiredException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return j.at("ok").get<bool>();
}

void foxogram::Me::deleteUser(std::string password) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/auth/delete",nlohmann::json({
        {"password", password} }), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}

bool foxogram::Me::confirmDeleteUser(std::string code) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/auth/delete/confirm"+code, token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return j.at("ok").get<bool>();
}

foxogram::User foxogram::Me::fetchMe() {
    auto j = foxogram::HttpClient::request(Payload("GET", "/users/@me", token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return {
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(),j.at("flags").get<long long>(),
            j.at("type"), j.at("createdAt").get<long long>()};
}

foxogram::Channel foxogram::Me::createChannel(std::string name, int type) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/create",
                                                   nlohmann::json({{"name", name}, {"type", type}}), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
            j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::joinChannel(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/"+std::to_string(id)+"/join", token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::ChannelNotFoundException();
            case(402): throw foxogram::MemberAlreadyInChannelException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                                     j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::fetchChannel(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("GET", "/channels/"+std::to_string(id), token));
    auto channel = foxogram::Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                                     j.at("type").get<int>(), j.at("ownerId").get<long long>());
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::ChannelNotFoundException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    channel.token = token;
    return channel;
}

foxogram::Message foxogram::Me::fetchMessage(long long int id) {
    auto j = foxogram::HttpClient::request(Payload("GET", "/messages/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(101): throw foxogram::MessageNotFoundException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    auto message = foxogram::Message(j.at("id").get<long long>(), nullptr, j.at("authorId").get<long long>(),
            j.at("timestamp").get<long long>(), j.at("attachments").get<std::list<std::string>>());
    message.token = token;
    return message;
}
