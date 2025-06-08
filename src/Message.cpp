#include <foxochat/Message.h>
#include <foxochat/Member.h>
#include <foxochat/exceptions.h>
#include <foxochat/HttpClient.h>
#include <foxochat/Utils.h>
#include <utility>

#include <foxochat/Attachment.h>

foxochat::Message::Message(long long int id, long long int channelId, long long int authorId,
                           long long int timestamp, std::string content, std::list<Attachment> attachments):
                           channelId(channelId), authorId(authorId), timestamp(timestamp),
                           attachments(std::move(attachments)), content(std::move(content)), BaseEntity(id) {}

void foxochat::Message::handleError(const nlohmann::json &response) const {
    if (!response.value("ok", true)) {
        switch (Utils::value<int>(response, "code", 0)) {
            case(301): throw UserUnauthorizatedException();
            case(302): throw UserEmailNotVerfiedException();
            case(403): throw MissingPermissionException();
            default: throw HttpException(Utils::value<std::string>(response, "message", ""));
        }
    }
}

void foxochat::Message::deleteMessage() const {
    handleError(HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(channelId) + "/messages/" + std::to_string(id), token)));
}

void foxochat::Message::edit() const {
    handleError(HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(channelId) + "/messages/" + std::to_string(id), token)));
}

long long int foxochat::Message::getChannelId() const {
    return channelId;
}

long long int foxochat::Message::getAuthorId() const {
    return authorId;
}

std::shared_ptr<foxochat::Member> foxochat::Message::getAuthor() const
{
    return author;
}


long long int foxochat::Message::getCreatedAt() const {
    return timestamp;
}

const std::list<foxochat::Attachment> &foxochat::Message::getAttachments() const {
    return attachments;
}

const std::string &foxochat::Message::getContent() const {
    return content;
}

std::shared_ptr<foxochat::Message> foxochat::Message::fromJSON(nlohmann::json j) {
    std::list<Attachment> attachments;
    for (const auto& attachment : j.at("attachments")) {
        attachments.push_back(Attachment::fromJson(attachment));
    }
    return std::make_shared<foxochat::Message>(Utils::value<long long>(j, "id", 0), Utils::value<long long>(j.at("channel"), "id", 0),
        Utils::value<long long>(j.at("author"), "id", 0), Utils::value<long long>(j, "created_at", 0),
        Utils::value<std::string>(j, "content", ""), attachments);
}
