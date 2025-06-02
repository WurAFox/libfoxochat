#include <foxogram/Serializer.h>
#include <foxogram/User.h>
#include <foxogram/Member.h>
#include <foxogram/Channel.h>
#include <foxogram/Message.h>
#include <foxogram/Utils.h>

foxogram::User nlohmann::adl_serializer<foxogram::User>::from_json(const nlohmann::json& j) {
    return {
        foxogram::Utils::value<long long>(j, "id", 0), foxogram::Utils::value<long long>(j, "created_at", 0),
        foxogram::Utils::value<std::string>(j, "username", ""), j.at("avatar").is_string() ?
        foxogram::Utils::value<std::string>(j, "avatar", "") : "", foxogram::Utils::value<long long>(j, "flags", 0),
        foxogram::Utils::value<int>(j, "type", 0), j.at("display_name").is_string() ?
        foxogram::Utils::value<std::string>(j, "display_name", "") : ""
    };
}

void nlohmann::adl_serializer<foxogram::User>::to_json(nlohmann::json &j, foxogram::User u) {
    j = {};
    j.at("id") = u.getId();
    j.at("username") = u.getUsername();
    j.at("avatar") = u.getAvatar();
    j.at("flags") = u.getFlags().toLongLong();
    j.at("type") = u.getType();
    j.at("display_name") = u.getDisplayName();
}

foxogram::Attachment nlohmann::adl_serializer<foxogram::Attachment>::from_json(
    const nlohmann::json& j) {
    return {
        foxogram::Utils::value<long long>(j, "id", 0),
        foxogram::Utils::value<std::string>(j, "uuid", ""), foxogram::Utils::value<std::string>(j, "filename", ""),
        foxogram::Utils::value<std::string>(j, "contentType", ""), foxogram::Utils::value<long long>(j, "flags", 0)
    };
}

void nlohmann::adl_serializer<foxogram::Attachment>::to_json(nlohmann::json& j,
    foxogram::Attachment a) {
    j.at("id") = a.getId();
    j.at("uuid") = a.getUuid();
    j.at("filename") = a.getFilename();
    j.at("contentType") = a.getContentType();
    j.at("flags") = a.getFlags().toLongLong();
}

foxogram::Member nlohmann::adl_serializer<foxogram::Member>::from_json(const nlohmann::json& j) {
    return {
        foxogram::Utils::value<long long>(j, "id", 0),
        j.contains("channel") ? foxogram::Utils::value<long long>(j.at("channel"), "id", 0) : 0,
        foxogram::Utils::value<long long>(j, "permissions", 0), foxogram::Utils::value<std::string>(j.at("user"), "username", ""),
        foxogram::Utils::value<std::string>(j.at("user"), "avatar", ""),
        foxogram::Utils::value<long long>(j.at("user"), "flags", 0),foxogram::Utils::value<int>(j.at("user"), "type", 0),
        foxogram::Utils::value<long long>(j.at("user"), "created_at", 0)
    };
}

void nlohmann::adl_serializer<foxogram::Member>::to_json(nlohmann::json& j, foxogram::Member a) {
    j = {};
    j.at("id") = a.getId();
    j.at("permissions") = a.getPermissions().toLongLong();
    j.at("user") = static_cast<foxogram::User>(a);
}

foxogram::Channel nlohmann::adl_serializer<foxogram::Channel>::from_json(const nlohmann::json& j) {
    return {
        foxogram::Utils::value<long long>(j, "id", 0),
        foxogram::Utils::value<std::string>(j, "name", ""),
        foxogram::Utils::value<std::string>(j, "display_name", ""), foxogram::Utils::value<short>(j, "type", 0),
        foxogram::Utils::value<std::string>(j.at("owner"), "username", ""),
        foxogram::Utils::value<long long>(j, "created_at", 0), foxogram::Utils::value<foxogram::Attachment>(j, "icon",
        foxogram::Attachment(0, "", "","", 0))
    };
}

void nlohmann::adl_serializer<foxogram::Channel>::to_json(nlohmann::json& j, foxogram::Channel a) {
    j = {};
    j.at("id") = a.getId();
    j.at("name") = a.getName();
    j.at("display_name") = a.getDisplayName();
    j.at("type") = a.getType();
    j.at("owner").at("username") = a.getOwnerName();
    j.at("created_at") = a.getCreatedAt();
    j.at("icon") = a.getIcon();
}

foxogram::Message nlohmann::adl_serializer<foxogram::Message>::from_json(const nlohmann::json& j) {
    return {
        foxogram::Utils::value<long long>(j, "id", 0), foxogram::Utils::value<long long>(j.at("channel"), "id", 0),
        foxogram::Utils::value<long long>(j.at("author"), "id", 0), foxogram::Utils::value<long long>(j, "created_at", 0),
        foxogram::Utils::value<std::string>(j, "content", ""), foxogram::Utils::value<std::list<foxogram::Attachment>>(j, "attachments", {})
    };
}

void nlohmann::adl_serializer<foxogram::Message>::to_json(nlohmann::json& j, foxogram::Message a) {
    j = {};
    j.at("id") = a.getId();
    j.at("channel").at("id") = a.getChannelId();
    // j.at("author") = a.getAuthor();
    j.at("created_at") = a.getCreatedAt();
    j.at("content") = a.getContent();
    j.at("attachments") = a.getAttachments();
}
