#include <foxogram/Channel.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>

#include <utility>

foxogram::Channel::Channel(std::string name, short type,
                           std::string ownerName, long long createdAt) : name(std::move(name)), type(type),
                                                          ownerName(ownerName), createdAt(createdAt) {
}

void foxogram::Channel::handleError(const nlohmann::json &response) const {
    if (!response.value("ok", true)) {
        switch (response.at("code").get<int>()) {
            case 301: throw UserUnauthorizatedException();
            case 302: throw UserEmailNotVerfiedException();
            case 401: throw MissingPermissionException();
            case 403: throw MemberInChannelNotFoundException();
            default: throw HttpException(response.at("message").get<std::string>());
        }
    }
}

void foxogram::Channel::deleteChannel() const {
    handleError(HttpClient::request(Payload("DELETE", "/channels/" + name, token)));
}

void foxogram::Channel::edit() const {
    handleError(HttpClient::request(Payload("PATCH", "/channels/" + name, token)));
}

void foxogram::Channel::leave() const {
    handleError(HttpClient::request(Payload("POST", "/channels/" + name + "/leave", token)));
}

std::list<foxogram::Message> foxogram::Channel::getMessages() const {
    const auto j = HttpClient::request(Payload("GET", "/channels/" + name, token));

    handleError(j);

    return {};
}

foxogram::Message foxogram::Channel::getMessage(const long long id) const {
    const auto j = HttpClient::request(
        Payload("GET", "/channels/" + name + "/" + std::to_string(id), token));

    handleError(j);

    return {0, nullptr, 0, 0, {}};
}

foxogram::Message foxogram::Channel::createMessage() const {
    const auto j = HttpClient::request(Payload("POST", "/channels/" + name, token));

    handleError(j);

    return {0, nullptr, 0, 0, {}};
}

const std::string &foxogram::Channel::getName() const {
    return name;
}

int foxogram::Channel::getType() const {
    return type;
}

const std::list<foxogram::Member> &foxogram::Channel::getMembers() const {
    return members;
}

std::string foxogram::Channel::getOwnerName() const {
    return name;
}