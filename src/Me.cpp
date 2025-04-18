#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <foxogram/Logger.h>
#include <foxogram/Utils.h>

foxogram::Me::Me(const std::string& _token) : User(fetchMe(token = new std::string(_token))), gateway(this) {
    channelCache = std::make_shared<Cache<Channel>>();
    userCache = std::make_shared<Cache<User>>();
}

foxogram::Me::Me(const std::string& username, const std::string& email, const std::string& password):
    User(fetchMe(token = new std::string(signup(username, email, password)))),
    gateway(this) {
    channelCache = std::make_shared<Cache<Channel>>();
    userCache = std::make_shared<Cache<User>>();
}

foxogram::Me::Me(const std::string& email, const std::string& password): User(fetchMe(token = new std::string(Me::login(email, password)))),
                                                                         gateway(this) {
    channelCache = std::make_shared<Cache<Channel>>();
    userCache = std::make_shared<Cache<User>>();
}

void foxogram::Me::handleError(const nlohmann::json &response) const {
    _handleError(response);
}

void foxogram::Me::_handleError(const nlohmann::json &response) {
    if (!response.value("ok", true)) {
        switch (Utils::value<int>(response, "code", 0)) {
            case 301: throw UserUnauthorizatedException();
            case 302: throw UserEmailNotVerfiedException();
            case 303: throw UserWithThisEmailAlreadyExistException();
            case 304: throw UserCredentialsIsInvalidException();
            case 502: throw CodeExpiredException();
            case 501: throw CodeIsInvalidException();
            default: throw HttpException(Utils::value<std::string>(response, "message", ""));
        }
    }
}

foxogram::UserPtr foxogram::Me::fetchUser(long long int id) {
    auto j = HttpClient::request(Payload("GET",
                                         std::string("/users/") + std::to_string(id), *token));
    handleError(j);

    auto user = User::fromJSON(j);
    return userCache->store(user);
}

std::string foxogram::Me::login(std::string email, std::string password) {
    Logger::logDebug("Attempt to login as " + email + " with password " + password);
    auto j = HttpClient::request(Payload("POST", "/auth/login", nlohmann::json({
                                                                                       {"email", email},
                                                                                       {"password", password}
                                                                               })));
    _handleError(j);
    Logger::logInfo("Logged in as " + email);
    return Utils::value<std::string>(j, "access_token", "");
}

std::string foxogram::Me::signup(std::string username, std::string email, std::string password) {
    Logger::logDebug("Attempt to signup as " + email + " username " + username + " with password " + password);
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/register", nlohmann::json({
                                                                                                  {"username", username},
                                                                                                  {"email", email},
                                                                                                  {"password", password}
                                                                                          })));
    _handleError(j);

    Logger::logInfo("Signed up in as " + email);
    return Utils::value<std::string>(j, "access_token", "");
}

bool foxogram::Me::verifyEmail(const std::string &code) const {
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/email/verify", nlohmann::json({{"code", code}}), *token));

    handleError(j);

    return Utils::value<bool>(j, "ok", true);
}

bool foxogram::Me::resendEmail() const {
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/email/resend", *token));

    handleError(j);

    return Utils::value<bool>(j, "ok", true);
}

bool foxogram::Me::deleteUser(std::string password) const {
    auto j = HttpClient::request(Payload("DELETE", "/users/@me", nlohmann::json({
                                                                                        {"password", password}
                                                                                }), *token));
    handleError(j);
    return Utils::value<bool>(j, "ok", true);
}

bool foxogram::Me::confirmDeleteUser(const std::string &code) const {
    auto j = HttpClient::request(Payload("POST", "/users/@me/delete-confirm", nlohmann::json({{"code", code}}), *token));
    handleError(j);

    return Utils::value<bool>(j, "ok", true);
}

foxogram::User foxogram::Me::fetchMe(std::string* token) {
    auto j = HttpClient::request(Payload("GET", "/users/@me", *token));
    handleError(j);
    return {
            Utils::value<long long>(j, "id", 0), Utils::value<long long>(j, "created_at", 0), Utils::value<std::string>(j, "username", ""),
            j.at("avatar").is_string() ? Utils::value<std::string>(j, "avatar", "") : "", Utils::value<long long>(j, "flags", 0),
            Utils::value<int>(j, "type", 0), j.at("created_at").is_string() ? Utils::value<std::string>(j, "created_at", "") : ""
    };
}

foxogram::User foxogram::Me::fetchMe() const {
    auto j = HttpClient::request(Payload("GET", "/users/@me", *token));
    handleError(j);
    return {
            Utils::value<long long>(j, "id", 0),Utils::value<long long>(j, "created_at", 0), Utils::value<std::string>(j, "username", ""),
            j.at("avatar").is_string() ? Utils::value<std::string>(j, "avatar", "") : "", Utils::value<long long>(j, "flags", 0),
            Utils::value<int>(j, "type", 0), j.at("created_at").is_string() ? Utils::value<std::string>(j, "created_at", "") : ""
    };
}

foxogram::ChannelPtr foxogram::Me::createChannel(std::string name, int type) {
    auto j = HttpClient::request(Payload("POST", "/channels/",
                                         nlohmann::json({{"name", name}, {"type", type}}), *token));
    handleError(j);
    auto channel = Channel::fromJSON(j);
    channel->token = *token;
    return channelCache->store(channel);
}

foxogram::ChannelPtr foxogram::Me::joinChannel(long long int id) {
    auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id) + "/join", *token));

    handleError(j);

    auto channel = Channel::fromJSON(j);
    channel->token = *token;
    return channelCache->store(channel);
}

foxogram::ChannelPtr foxogram::Me::fetchChannel(long long int id) {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id), *token));
    handleError(j);
    auto channel = Channel::fromJSON(j);
    channel->token = *token;
    return channelCache->store(channel);
}


foxogram::Me::~Me() {
    gateway.close();
    delete token;
    delete MessageCreate;
    delete MessageUpdate;
    delete MessageDelete;
    delete ChannelCreate;
    delete ChannelUpdate;
    delete ChannelDelete;
    delete MemberAdd;
    delete MemberRemove;
    delete MemberUpdate;
    delete Hello;
    delete Pong;
}

foxogram::ChannelPtr foxogram::Me::getChannel(long long id) {
    return channelCache->get(id);
}

foxogram::UserPtr foxogram::Me::getUser(long long id) {
    return userCache->get(id);
}