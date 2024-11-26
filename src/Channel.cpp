#include <foxogram/Channel.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>

#include <utility>

foxogram::Channel::Channel(const long long int id, std::string name, const int type,
                           const long long int ownerId) : BaseEntity(id), name(std::move(name)), type(type),
                                                          ownerId(ownerId) {
}

void foxogram::Channel::handleError(const nlohmann::json &response) const {
    if (!response.at("ok").get<bool>()) {
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
    handleError(HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id), token)));
}

void foxogram::Channel::edit() const {
    handleError(HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id), token)));
}

void foxogram::Channel::leave() const {
    handleError(HttpClient::request(Payload("POST", "/channels/" + std::to_string(id) + "/leave", token)));
}

std::list<foxogram::Message> foxogram::Channel::getMessages() const {
    const auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(this->id), token));

    handleError(j);

    return {};
}

foxogram::Message foxogram::Channel::getMessage(const long long id) const {
    const auto j = HttpClient::request(
        Payload("GET", "/channels/" + std::to_string(this->id) + "/" + std::to_string(id), token));

    handleError(j);

    return {0, nullptr, 0, 0, {}};
}

foxogram::Message foxogram::Channel::createMessage() const {
    const auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id), token));

    handleError(j);

    return {0, nullptr, 0, 0, {}};
}

const std::string &foxogram::Channel::getName() const {
    return name;
}

int foxogram::Channel::getType() const {
    return type;
}

long long int foxogram::Channel::getOwnerId() const {
    return ownerId;
}

const std::list<foxogram::Member> &foxogram::Channel::getMembers() const {
    return members;
}
