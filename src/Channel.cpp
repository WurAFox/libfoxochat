#include <foxochat/Channel.h>
#include <foxochat/HttpClient.h>
#include <foxochat/exceptions.h>
#include <foxochat/Utils.h>
#include <utility>
#include <foxochat/Logger.h>

foxochat::Channel::Channel(long long id, std::string name, std::string displayName, short type,
                           std::string ownerName, long long createdAt, Attachment icon) : BaseEntity(id), name(std::move(name)),
    type(type), ownerName(std::move(ownerName)), createdAt(createdAt), displayName(std::move(displayName)), icon(std::move(icon)) {
    messages = std::make_shared<foxochat::Cache<Message>>();
    members = std::make_shared<foxochat::Cache<Member>>();
}

void foxochat::Channel::handleError(const nlohmann::json &response) const {
    if (!response.value("ok", true)) {
        switch (Utils::value<int>(response, "code", 0)) {
            case 301: throw UserUnauthorizatedException();
            case 302: throw UserEmailNotVerfiedException();
            case 401: throw MissingPermissionException();
            case 403: throw MemberInChannelNotFoundException();
            default: throw HttpException(Utils::value<std::string>(response, "message", ""));
        }
    }
}

bool foxochat::Channel::deleteChannel() {
    auto j = HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id), token));
    handleError(j);
    return Utils::value<bool>(j, "ok", true);
}

void foxochat::Channel::edit(const std::string& displayName, const std::string& name, const Attachment& icon) {
    auto displayName_ = displayName;
    auto name_ = name;
    auto icon_ = icon;

    if (displayName.empty()) {
        displayName_ = this->displayName;
    }

    if (name.empty()) {
        name_ = this->name;
    }

    if (icon.getUuid().empty()) {
        icon_ = this->icon;
    }

    auto j = HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id),
        nlohmann::json{{"name", name_}, {"displayName", displayName_}, {"icon", icon_.getUuid()}}, token));
    handleError(j);
    this->displayName = displayName_;
    this->name = name_;
    this->icon = icon_;
}

bool foxochat::Channel::leave() {
    auto j =HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id) + "/member/@me", token));
    handleError(j);
    return Utils::value<bool>(j, "ok", true);
}

std::list<foxochat::MessagePtr> foxochat::Channel::getMessages() const {
    std::list<foxochat::MessagePtr> messageList;
    std::transform(messages->getMap().begin(), messages->getMap().end(),
        std::back_inserter(messageList), [](const std::pair<long long, std::shared_ptr<Message>>& p) {return p.second;});
    return messageList;
}

std::list<foxochat::MessagePtr> foxochat::Channel::fetchMessages(long long before, int limit) {
    const auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id) + "/messages?before=" +
        std::to_string(before) +"&limit=" + std::to_string(limit), token));
    if (!j.is_array()) {
        handleError(j);
    }
    for (auto& message : j) {
        auto member = foxochat::Member::fromJSON(message.at("author"), id);
        auto msg = foxochat::Message::fromJSON(message);
        msg->token = token;
        msg->author = members->store(member);
        messages->store(msg);
    }

    std::list<foxochat::MessagePtr> messageList;
    std::transform(messages->getMap().begin(), messages->getMap().end(),
        std::back_inserter(messageList), [](const std::pair<long long, std::shared_ptr<Message>>& p) {return p.second;});
    return messageList;
}

foxochat::MessagePtr foxochat::Channel::fetchMessage(long long id) {
    const auto j = HttpClient::request(
        Payload("GET", "/channels/" + std::to_string(this->id) + "/messages/" + std::to_string(id), token));

    handleError(j);
    auto msg = foxochat::Message::fromJSON(j);
    auto member = foxochat::Member::fromJSON(j.at("author"), id);
    msg->token = token;
    msg->author = members->store(member);
    return messages->store(msg);
}

std::list<foxochat::MemberPtr> foxochat::Channel::fetchMembers() {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id) + "/members", token));
    handleError(j);
    for (const auto& member : j) {
        members->store(foxochat::Member::fromJSON(member, id));
    }
    std::list<foxochat::MemberPtr> memberList;
    std::transform(members->getMap().begin(), members->getMap().end(),
        std::back_inserter(memberList), [](const std::pair<long long, std::shared_ptr<Member>>& p) {return p.second;});
    return memberList;
}

foxochat::MemberPtr foxochat::Channel::fetchMember(long long id) {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(this->id)
    + "/members/" + std::to_string(id), token));
    handleError(j);
    auto member = foxochat::Member::fromJSON(j, id);
    return members->store(member);;
}

foxochat::MemberPtr foxochat::Channel::getMember(long long id) {
    return members->get(id);
}

foxochat::MessagePtr foxochat::Channel::createMessage(std::string content, const std::list<long long>& attachments) {
    const auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id) + "/messages",
        nlohmann::json{{"content", content}, {"attachments", attachments}}, token));

    handleError(j);
    auto msg = foxochat::Message::fromJSON(j);
    auto member = foxochat::Member::fromJSON(j.at("author"), id);
    msg->token = token;
    msg->author = members->store(member);
    return messages->store(msg);
}

std::string foxochat::Channel::getName() const {
    return name;
}

int foxochat::Channel::getType() const {
    return type;
}

std::list<foxochat::MemberPtr> foxochat::Channel::getMembers() const {
    std::list<foxochat::MemberPtr> memberList;
    std::transform(members->getMap().begin(), members->getMap().end(),
        std::back_inserter(memberList), [](const std::pair<long long, std::shared_ptr<Member>>& p) {return p.second;});
    return memberList;
}

std::string foxochat::Channel::getOwnerName() const {
    return ownerName;
}

long long foxochat::Channel::getCreatedAt() const {
    return createdAt;
}

const std::string &foxochat::Channel::getDisplayName() const {
    return displayName;
}

const foxochat::Attachment& foxochat::Channel::getIcon() const {
    return icon;
}

std::shared_ptr<foxochat::Channel> foxochat::Channel::fromJSON(nlohmann::json j) {
    return std::make_shared<Channel>(Utils::value<long long>(j, "id", 0),
        Utils::value<std::string>(j, "name", ""),
        Utils::value<std::string>(j, "display_name", ""), Utils::value<short>(j, "type", 0),
        Utils::value<std::string>(j.at("owner"), "username", ""),
        Utils::value<long long>(j, "created_at", 0), Utils::value<Attachment>(j, "icon", Attachment{0, "", "", "", 0}));
}