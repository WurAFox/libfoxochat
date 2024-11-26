#include <foxogram/Channel.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>

foxogram::Channel::Channel(long long int id, std::string name, int type, long long int ownerId) : id(id),
                                                                                                         name(name),
                                                                                                         type(type),
                                                                                                         ownerId(ownerId) {
    this->id = id;
    this->name = std::move(name);
    this->type = type;
    this->ownerId = ownerId;
}

void foxogram::Channel::deleteChannel() {
    auto j = foxogram::HttpClient::request(Payload("DELETE", "/channels/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(403): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}

void foxogram::Channel::edit() {
    auto j = foxogram::HttpClient::request(Payload("PATCH", "/channels/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}

void foxogram::Channel::leave() {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/"+std::to_string(id)+"leave", token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MemberInChannelNotFoundException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}

std::list<foxogram::Message> foxogram::Channel::getMessages() {
    auto j = foxogram::HttpClient::request(Payload("GET", "/channels/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return {};
}

foxogram::Message foxogram::Channel::getMessage(long long id) {
    auto j = foxogram::HttpClient::request(Payload("GET",
                                                   "/channels/"+std::to_string(this->id)+"/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return {0, nullptr, 0, 0, {}};
}

foxogram::Message foxogram::Channel::createMessage() {
    auto j = foxogram::HttpClient::request(Payload("POST", "/channels/"+std::to_string(id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
    return {0, nullptr, 0, 0, {}};
}

long long int foxogram::Channel::getId() const {
    return id;
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
