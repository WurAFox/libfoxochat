#include <foxogram/Channel.h>
#include <foxogram/HttpClient.h>
#include <foxogram/exceptions.h>
#include <utility>

foxogram::Channel::Channel(long long id, std::string name, std::string displayName, short type,
                           std::string ownerName, long long createdAt, std::string icon) : name(std::move(name)), displayName(std::move(displayName)),
                           type(type), ownerName(std::move(ownerName)), createdAt(createdAt), icon(std::move(icon)), BaseEntity(id) {
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

bool foxogram::Channel::deleteChannel() {
    auto j = HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id), token));
    handleError(j);
    return j.at("ok").get<bool>();
}

void foxogram::Channel::edit(const std::string& displayName, const std::string& name, const std::string& icon) {
    auto j = HttpClient::request(Payload("PATCH", "/channels/" + std::to_string(id), nlohmann::json{
        {"displayName", displayName},
        {"name", name},
        {"icon", icon}
    }, token));
    handleError(j);
    this->displayName = displayName;
    this->name = name;
    this->icon = icon;
}

bool foxogram::Channel::leave() {
    auto j =HttpClient::request(Payload("DELETE", "/channels/" + std::to_string(id) + "/member/@me", token));
    handleError(j);
    return j.at("ok").get<bool>();
}

std::list<foxogram::MessagePtr> foxogram::Channel::getMessages() const {
    std::list<foxogram::MessagePtr> messagesList;
    for (auto& message : messages) {
        messagesList.push_back(message.second);
    }

    return messagesList;
}

std::list<foxogram::MessagePtr> foxogram::Channel::fetchMessages() {
    const auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id), token));
    handleError(j);
    for (auto& message : j) {
        messages.insert_or_assign(message.at("id").get<long long>(),
            std::make_shared<foxogram::Message>(message.at("id").get<long long>(), message.at("channel").get<long long>(),
            message.at("author").at("id").get<long long>(), message.at("created_at").get<long long>(),
            message.at("content").get<std::string>(), message.at("attachments").get<std::list<std::string>>()));
    }

    std::list<foxogram::MessagePtr> messagesList;
    for (auto& message : messages) {
        messagesList.push_back(message.second);
    }
    return messagesList;
}

foxogram::MessagePtr foxogram::Channel::fetchMessage(long long id) {
    const auto j = HttpClient::request(
        Payload("GET", "/channels/" + std::to_string(this->id) + "/" + std::to_string(id), token));

    handleError(j);
    auto msg = std::make_shared<foxogram::Message>(j.at("id").get<long long>(), j.at("channel").at("id").get<long long>(),
            j.at("author").at("id").get<long long>(), j.at("created_at").get<long long>(),
            j.at("content").get<std::string>(), j.at("attachments").get<std::list<std::string>>());
    messages.insert_or_assign(j.at("id").get<long long>(), msg);
    return msg;
}

std::list<foxogram::MemberPtr> foxogram::Channel::fetchMembers() {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(id) + "/members", token));
    handleError(j);
    members.clear();
    for (auto member : j) {
        members.insert_or_assign(member.at("id").get<long long>(), std::make_shared<foxogram::Member>(member.at("id").get<long long>(), id,
                 member.at("permission").get<long long>(), member.at("user").at("username").get<std::string>(),
                 member.at("user").at("avatar").get<std::string>(),member.at("user").at("flags").get<long long>(),
                 member.at("user").at("type").get<int>(),member.at("user").at("created_at").get<long long>()));
    }
    std::list<foxogram::MemberPtr> membersList;
    for(auto const& p: members) {
        membersList.push_back(p.second);
    }
    return membersList;
}

foxogram::MemberPtr foxogram::Channel::fetchMember(long long id) {
    auto j = HttpClient::request(Payload("GET", "/channels/" + std::to_string(this->id)
    + "/members/" + std::to_string(id), token));
    handleError(j);
    auto member = std::make_shared<foxogram::Member>(j.at("id").get<long long>(), id,
        j.at("permission").get<long long>(), j.at("user").at("username").get<std::string>(),
        j.at("user").at("avatar").get<std::string>(),j.at("user").at("flags").get<long long>(),
        j.at("user").at("type").get<int>(),j.at("user").at("created_at").get<long long>());
    members.insert_or_assign(j.at("id").get<long long>(), member);
    return member;
}

foxogram::MemberPtr foxogram::Channel::getMember(long long id) {
    auto it = members.find(id);
    return (it != members.end()) ? it->second : nullptr;
}

foxogram::MessagePtr foxogram::Channel::createMessage(std::string content, const std::list<std::string>& attachments) {
    const auto j = HttpClient::request(Payload("POST", "/channels/" + std::to_string(id),
        nlohmann::json({{"content", content}, {attachments, attachments}}), token));

    handleError(j);
    auto msg = std::make_shared<foxogram::Message>(j.at("id").get<long long>(), j.at("channel").at("id").get<long long>(),
        j.at("author").at("id").get<long long>(), j.at("created_at").get<long long>(),
        j.at("content").get<std::string>(), j.at("attachments").get<std::list<std::string>>());
    messages.insert_or_assign(j.at("id").get<long long>(), msg);
    return msg;
}

std::string foxogram::Channel::getName() const {
    return name;
}

int foxogram::Channel::getType() const {
    return type;
}

std::list<foxogram::MemberPtr> foxogram::Channel::getMembers() const {
    std::list<foxogram::MemberPtr> membersList;
    for(auto const& p: members) {
        membersList.push_back(p.second);
    }
    return membersList;
}

std::string foxogram::Channel::getOwnerName() const {
    return ownerName;
}
