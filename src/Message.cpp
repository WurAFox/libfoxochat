#include <foxogram/Message.h>
#include <foxogram/Member.h>
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
    handleError(HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(channelId) + std::to_string(id), token)));
}

void foxogram::Message::edit() const {
    handleError(HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id), token)));
}

long long int foxogram::Message::getChannelId() const {
    return channelId;
}

long long int foxogram::Message::getAuthorId() const {
    return authorId;
}

long long int foxogram::Message::getCreatedAt() const {
    return timestamp;
}

const std::list<std::string> &foxogram::Message::getAttachments() const {
    return attachments;
}

const std::string &foxogram::Message::getContent() const {
    return content;
}

std::shared_ptr<foxogram::Message> foxogram::Message::fromJSON(nlohmann::json j) {
    return std::make_shared<foxogram::Message>(j.at("id").get<long long>(), j.at("channel").at("id").get<long long>(),
        j.at("author").at("id").get<long long>(), j.at("created_at").get<long long>(),
        j.at("content").get<std::string>(), j.at("attachments").get<std::list<std::string>>());
}
