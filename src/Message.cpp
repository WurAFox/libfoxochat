#include <foxogram/Message.h>
#include <foxogram/Channel.h>
#include <foxogram/exceptions.h>
#include <foxogram/HttpClient.h>
#include <utility>

foxogram::Message::Message(long long int id, foxogram::Channel *channel, long long int authorId, long long int timestamp,
                           std::list<std::string> attachments) {
    this->id = id;
    this->channel = channel;
    this->authorId = authorId;
    this->timestamp = timestamp;
    this->attachments = std::move(attachments);
}

void foxogram::Message::deleteMessage() {
    auto j = foxogram::HttpClient::request(Payload("DELETE", "/channels/"+
    std::to_string(this->channel->getId())+std::to_string(this->id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(403): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}

void foxogram::Message::edit() {
    auto j = foxogram::HttpClient::request(Payload("PATCH", "/channels/"+std::to_string(this->id), token));
    if (!j.at("ok").get<bool>()) {
        switch (j.at("code").get<int>()) {
            case(301): throw foxogram::UserUnauthorizatedException();
            case(302): throw foxogram::UserEmailNotVerfiedException();
            case(401): throw foxogram::MissingPermissionException();
            default: throw HttpException(j.at("message").get<std::string>());
        }
    }
}
