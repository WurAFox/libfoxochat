#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <foxogram/Logger.h>
#include <utility>

foxogram::Me::Me(const std::string& token) : User(fetchMe(token)), token(token) {
}

foxogram::Me::Me(std::string username, const std::string& email, const std::string& password): User(fetchMe(Me::signup(std::move(std::move(username)), email, password))),
    token(Me::login(email, password)) {
}

foxogram::Me::Me(const std::string& email, const std::string& password): User(fetchMe(Me::login(email, password))), token(Me::login(email, password)) {
}

void foxogram::Me::handleError(const nlohmann::json &response) const {
    _handleError(response);
}

void foxogram::Me::_handleError(const nlohmann::json &response) {
    if (!response.value("ok", true)) {
        switch (response.at("code").get<int>()) {
            case 301: throw UserUnauthorizatedException();
            case 302: throw UserEmailNotVerfiedException();
            case 303: throw UserWithThisEmailAlreadyExistException();
            case 304: throw UserCredentialsIsInvalidException();
            case 503: throw CodeExpiredException();
            case 501: throw CodeIsInvalidException();
            default: throw HttpException(response.at("message").get<std::string>());
        }
    }
}

foxogram::User foxogram::Me::fetchUser(const long long id) const {
    auto j = HttpClient::request(Payload("GET",
                                         std::string("/users/") + std::to_string(id), this->token));
    handleError(j);

    return {
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(), j.at("flags").get<long long>(),
            j.at("type")
    };
}

std::string foxogram::Me::login(std::string email, std::string password) {
    Logger::logDebug("Attempt to login as " + email + " with password " + password);
    auto j = HttpClient::request(Payload("POST", "/auth/login", nlohmann::json({
                                                                                       {"email", email},
                                                                                       {"password", password}
                                                                               })));
    _handleError(j);
    Logger::logInfo("Logged in as " + email);
    return j.at("accessToken").get<std::string>();
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    Logger::logDebug("Attempt to signup as " + email + " username " + username + " with password " + password);
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/signup", nlohmann::json({
                                                                                                  {"username", username},
                                                                                                  {"email", email},
                                                                                                  {"password", password}
                                                                                          })));
    _handleError(j);

    Logger::logInfo("Signed up in as " + email);
    return j.at("accessToken").get<std::string>();
}

bool foxogram::Me::verifyEmail(const std::string &code) const {
    nlohmann::json j = HttpClient::request(Payload("POST", "/v1/auth/email/verify/" + code, token));

    handleError(j);

    return j.at("ok").get<bool>();
}

void foxogram::Me::deleteUser(std::string password) const {
    auto j = HttpClient::request(Payload("POST", "/auth/delete", nlohmann::json({
                                                                                        {"password", password}
                                                                                }), token));
    handleError(j);
}

bool foxogram::Me::confirmDeleteUser(const std::string &code) const {
    auto j = HttpClient::request(Payload("POST", "/auth/delete/confirm/" + code, token));

    handleError(j);

    return j.at("ok").get<bool>();
}

foxogram::User foxogram::Me::fetchMe(const std::string& token) {
    auto j = HttpClient::request(Payload("GET", "/users/@me", token));
    handleError(j);
    return {
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(), j.at("flags").get<long long>(),
            j.at("type")
    };
}

foxogram::User foxogram::Me::fetchMe() const {
    auto j = HttpClient::request(Payload("GET", "/users/@me", this->token));
    handleError(j);
    return {
            j.at("id").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").get<std::string>(), j.at("flags").get<long long>(),
            j.at("type")
    };
}

foxogram::Channel foxogram::Me::createChannel(std::string name, int type) const {
    auto j = HttpClient::request(Payload("POST", "/channels/create",
                                         nlohmann::json({{"name", name}, {"type", type}}), token));

    handleError(j);

    auto channel = Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                           j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::joinChannel(const long long int id) const {
    auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id) + "/join", token));

    handleError(j);

    auto channel = Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                           j.at("type").get<int>(), j.at("ownerId").get<long long>());
    channel.token = token;
    return channel;
}

foxogram::Channel foxogram::Me::fetchChannel(const long long int id) const {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id), token));
    auto channel = Channel(j.at("id").get<long long>(), j.at("name").get<std::string>(),
                           j.at("type").get<int>(), j.at("ownerId").get<long long>());

    handleError(j);

    channel.token = token;
    return channel;
}

foxogram::Message foxogram::Me::fetchMessage(const long long int id) const {
    auto j = HttpClient::request(Payload("GET", "/messages/" + std::to_string(id), token));

    handleError(j);

    auto message = Message(j.at("id").get<long long>(), nullptr, j.at("authorId").get<long long>(),
                           j.at("timestamp").get<long long>(),
                           j.at("attachments").get<std::list<std::string> >());
    message.token = token;
    return message;
}
