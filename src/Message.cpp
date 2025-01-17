#include <foxogram/Message.h>
#include <foxogram/Channel.h>
#include <foxogram/exceptions.h>
#include <foxogram/HttpClient.h>
#include <utility>

foxogram::Message::Message(long long int id, long long int channelId, long long int authorId,
                           long long int timestamp, std::string content, std::list<std::string> attachments):
                           channelId(channelId), authorId(authorId), timestamp(timestamp),
                           attachments(std::move(attachments)), content(std::move(content)), BaseEntity(id) {}

void foxogram::Message::handleError(const nlohmann::json &response) const {
    if (!response.value("ok", true)) {
        switch (response.at("code").get<int>()) {
            case(301): throw UserUnauthorizatedException();
            case(302): throw UserEmailNotVerfiedException();
            case(403): throw MissingPermissionException();
            default: throw HttpException(response.at("message").get<std::string>());
        }
    }
}

void foxogram::Message::deleteMessage() const {
    handleError(HttpClient::request(Payload("DELETE", "/channels/" +
                                                                channel->getName() + std::to_string(id), token)));
}

void foxogram::Message::edit() const {
    handleError(HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id), token)));
}
