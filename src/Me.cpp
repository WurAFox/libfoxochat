#include <foxogram/Me.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <foxogram/Logger.h>

#include <utility>

foxogram::Me::Me(const std::string& _token) : User(fetchMe(token = new std::string(_token))) {
    gateway = new foxogram::Gateway(this);
}

foxogram::Me::Me(const std::string& username, const std::string& email, const std::string& password):
    User(fetchMe(token = new std::string(signup(username, email, password)))) {
    gateway = new foxogram::Gateway(this);
}

foxogram::Me::Me(const std::string& email, const std::string& password): User(fetchMe(token = new std::string(Me::login(email, password)))) {
    gateway = new foxogram::Gateway(this);
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
            case 502: throw CodeExpiredException();
            case 501: throw CodeIsInvalidException();
            default: throw HttpException(response.at("message").get<std::string>());
        }
    }
}

foxogram::UserPtr foxogram::Me::fetchUser(const std::string& username) {
    auto j = HttpClient::request(Payload("GET",
                                         std::string("/users/") +username, *token));
    handleError(j);

    auto user = std::make_shared<User>(
            j.at("id").get<long long>(), j.at("created_at").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").is_string() ? j.at("avatar").get<std::string>() : "", j.at("flags").get<long long>(),
            j.at("type").get<int>(), j.at("created_at").is_string() ? j.at("created_at").get<std::string>() : ""
    );
    return user;
}

std::string foxogram::Me::login(std::string email, std::string password) {
    Logger::logDebug("Attempt to login as " + email + " with password " + password);
    auto j = HttpClient::request(Payload("POST", "/auth/login", nlohmann::json({
                                                                                       {"email", email},
                                                                                       {"password", password}
                                                                               })));
    _handleError(j);
    Logger::logInfo("Logged in as " + email);
    return j.at("access_token").get<std::string>();
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
    return j.at("access_token").get<std::string>();
}

bool foxogram::Me::verifyEmail(const std::string &code) const {
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/email/verify", nlohmann::json({{"code", code}}), *token));

    handleError(j);

    return j.at("ok").get<bool>();
}

bool foxogram::Me::resendEmail() const {
    nlohmann::json j = HttpClient::request(Payload("POST", "/auth/email/resend", *token));

    handleError(j);

    return j.at("ok").get<bool>();
}

bool foxogram::Me::deleteUser(std::string password) const {
    auto j = HttpClient::request(Payload("DELETE", "/users/@me", nlohmann::json({
                                                                                        {"password", password}
                                                                                }), *token));
    handleError(j);
    return j.at("ok").get<bool>();
}

bool foxogram::Me::confirmDeleteUser(const std::string &code) const {
    auto j = HttpClient::request(Payload("POST", "/users/@me/delete-confirm", nlohmann::json({{"code", code}}), *token));
    handleError(j);

    return j.at("ok").get<bool>();
}

foxogram::User foxogram::Me::fetchMe(std::string* token) {
    auto j = HttpClient::request(Payload("GET", "/users/@me", *token));
    handleError(j);
    return {
            j.at("id").get<long long>(), j.at("created_at").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").is_string() ? j.at("avatar").get<std::string>() : "", j.at("flags").get<long long>(),
            j.at("type").get<int>(), j.at("created_at").is_string() ? j.at("created_at").get<std::string>() : ""
    };
}

foxogram::User foxogram::Me::fetchMe() const {
    auto j = HttpClient::request(Payload("GET", "/users/@me", *token));
    handleError(j);
    return {
            j.at("id").get<long long>(),j.at("created_at").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").is_string() ? j.at("avatar").get<std::string>() : "", j.at("flags").get<long long>(),
            j.at("type").get<int>(), j.at("created_at").is_string() ? j.at("created_at").get<std::string>() : ""
    };
}

foxogram::ChannelPtr foxogram::Me::createChannel(std::string name, int type) {
    auto j = HttpClient::request(Payload("POST", "/channels/create",
                                         nlohmann::json({{"name", name}, {"type", type}}), *token));

    handleError(j);

    auto channel = std::make_shared<Channel>(j.at("id").get<long long>(),
        j.at("name").get<std::string>(), j.at("display_name").get<std::string>(), j.at("type").get<int>(),
        j.at("owner").get<std::string>(), j.at("created_at").get<long long>(), j.at("icon").get<std::string>());
    channel->token = *token;
    return channel;
}

foxogram::ChannelPtr foxogram::Me::joinChannel(std::string name) {
    auto j = HttpClient::request(Payload("POST", "/channels/" + name + "/join", *token));

    handleError(j);

    auto channel = std::make_shared<Channel>(j.at("id").get<long long>(),
            j.at("name").get<std::string>(), j.at("display_name").get<std::string>(), j.at("type").get<int>(),
            j.at("owner").get<std::string>(), j.at("created_at").get<long long>(), j.at("icon").get<std::string>());
    channel->token = *token;
    return channel;
}

foxogram::ChannelPtr foxogram::Me::fetchChannel(std::string name) {
    auto j = HttpClient::request(Payload("GET", "/channels/" + name, *token));
    auto channel = std::make_shared<Channel>(j.at("id").get<long long>(),
            j.at("name").get<std::string>(), j.at("display_name").get<std::string>(), j.at("type").get<int>(),
            j.at("owner").get<std::string>(), j.at("created_at").get<long long>(), j.at("icon").get<std::string>());
    handleError(j);

    channel->token = *token;
    return channel;
}


foxogram::Me::~Me() {
    gateway->close();
    delete gateway;
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
