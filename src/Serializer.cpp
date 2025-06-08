#include <foxochat/Serializer.h>
#include <foxochat/User.h>
#include <foxochat/Member.h>
#include <foxochat/Channel.h>
#include <foxochat/Message.h>
#include <foxochat/Utils.h>

foxochat::User nlohmann::adl_serializer<foxochat::User>::from_json(const nlohmann::json& j) {
    return {
        foxochat::Utils::value<long long>(j, "id", 0), foxochat::Utils::value<long long>(j, "created_at", 0),
        foxochat::Utils::value<std::string>(j, "username", ""),
        foxochat::Utils::value<foxochat::Attachment>(j, "avatar", {0, "", "", "", 0}), foxochat::Utils::value<long long>(j, "flags", 0),
        foxochat::Utils::value<int>(j, "type", 0), j.at("display_name").is_string() ?
        foxochat::Utils::value<std::string>(j, "display_name", "") : ""
    };
}

void nlohmann::adl_serializer<foxochat::User>::to_json(nlohmann::json &j, foxochat::User u) {
    j = {};
    j.at("id") = u.getId();
    j.at("username") = u.getUsername();
    j.at("avatar") = u.getAvatar();
    j.at("flags") = u.getFlags().toLongLong();
    j.at("type") = u.getType();
    j.at("display_name") = u.getDisplayName();
}

foxochat::Attachment nlohmann::adl_serializer<foxochat::Attachment>::from_json(
    const nlohmann::json& j) {
    return {
        foxochat::Utils::value<long long>(j, "id", 0),
        foxochat::Utils::value<std::string>(j, "uuid", ""), foxochat::Utils::value<std::string>(j, "filename", ""),
        foxochat::Utils::value<std::string>(j, "contentType", ""), foxochat::Utils::value<long long>(j, "flags", 0)
    };
}

void nlohmann::adl_serializer<foxochat::Attachment>::to_json(nlohmann::json& j,
    foxochat::Attachment a) {
    j.at("id") = a.getId();
    j.at("uuid") = a.getUuid();
    j.at("filename") = a.getFilename();
    j.at("contentType") = a.getContentType();
    j.at("flags") = a.getFlags().toLongLong();
}

foxochat::Member nlohmann::adl_serializer<foxochat::Member>::from_json(const nlohmann::json& j) {
    return {
        foxochat::Utils::value<long long>(j, "id", 0),
        j.contains("channel") ? foxochat::Utils::value<long long>(j.at("channel"), "id", 0) : 0,
        foxochat::Utils::value<long long>(j, "permissions", 0), foxochat::Utils::value<std::string>(j.at("user"), "username", ""),
        foxochat::Utils::value<foxochat::Attachment>(j.at("user"), "avatar", {0, "", "", "", 0}),
        foxochat::Utils::value<long long>(j.at("user"), "flags", 0),foxochat::Utils::value<int>(j.at("user"), "type", 0),
        foxochat::Utils::value<long long>(j.at("user"), "created_at", 0)
    };
}

void nlohmann::adl_serializer<foxochat::Member>::to_json(nlohmann::json& j, foxochat::Member a) {
    j = {};
    j.at("id") = a.getId();
    j.at("permissions") = a.getPermissions().toLongLong();
    j.at("user") = static_cast<foxochat::User>(a);
}

foxochat::Channel nlohmann::adl_serializer<foxochat::Channel>::from_json(const nlohmann::json& j) {
    return {
        foxochat::Utils::value<long long>(j, "id", 0),
        foxochat::Utils::value<std::string>(j, "name", ""),
        foxochat::Utils::value<std::string>(j, "display_name", ""), foxochat::Utils::value<short>(j, "type", 0),
        foxochat::Utils::value<std::string>(j.at("owner"), "username", ""),
        foxochat::Utils::value<long long>(j, "created_at", 0), foxochat::Utils::value<foxochat::Attachment>(j, "icon",
        foxochat::Attachment(0, "", "","", 0))
    };
}

void nlohmann::adl_serializer<foxochat::Channel>::to_json(nlohmann::json& j, foxochat::Channel a) {
    j = {};
    j.at("id") = a.getId();
    j.at("name") = a.getName();
    j.at("display_name") = a.getDisplayName();
    j.at("type") = a.getType();
    j.at("owner").at("username") = a.getOwnerName();
    j.at("created_at") = a.getCreatedAt();
    j.at("icon") = a.getIcon();
}

foxochat::Message nlohmann::adl_serializer<foxochat::Message>::from_json(const nlohmann::json& j) {
    return {
        foxochat::Utils::value<long long>(j, "id", 0), foxochat::Utils::value<long long>(j.at("channel"), "id", 0),
        foxochat::Utils::value<long long>(j.at("author"), "id", 0), foxochat::Utils::value<long long>(j, "created_at", 0),
        foxochat::Utils::value<std::string>(j, "content", ""), foxochat::Utils::value<std::list<foxochat::Attachment>>(j, "attachments", {})
    };
}

void nlohmann::adl_serializer<foxochat::Message>::to_json(nlohmann::json& j, foxochat::Message a) {
    j = {};
    j.at("id") = a.getId();
    j.at("channel").at("id") = a.getChannelId();
    // j.at("author") = a.getAuthor();
    j.at("created_at") = a.getCreatedAt();
    j.at("content") = a.getContent();
    j.at("attachments") = a.getAttachments();
}
