#include <foxochat/Member.h>
#include <foxochat/Utils.h>

#include <utility>

foxochat::Member::Member(long long id, long long int channelId, long long permissions, const std::string &username,
                         Attachment avatar, long long flags, int type, long long createdAt)
    : User(id, createdAt, username, std::move(avatar), flags, type),
      channelId(channelId), permissions(permissions) {
}

void foxochat::Member::addPermissions(const MemberPermissions permission) {
    permissions.addFlag(permission);
}

bool foxochat::Member::hasPermissions(const MemberPermissions permission) const {
    return permissions.hasFlag(permission);
}

void foxochat::Member::removePermissions(const MemberPermissions permission) {
    permissions.removeFlag(permission);
}

long long int foxochat::Member::getChannelId() const {
    return channelId;
}

const foxochat::FlagsBase<foxochat::MemberPermissions> &foxochat::Member::getPermissions() const {
    return permissions;
}

std::shared_ptr<foxochat::Member> foxochat::Member::fromJSON(nlohmann::json j, long long channelId) {
    return std::make_shared<foxochat::Member>(Utils::value<long long>(j, "id", 0),
        j.contains("channel") ? Utils::value<long long>(j.at("channel"), "id", 0) : channelId,
        Utils::value<long long>(j, "permissions", 0), Utils::value<std::string>(j.at("user"), "username", ""),
        Utils::value<Attachment>(j.at("user"), "avatar", {0, "", "", "", 0}),
        Utils::value<long long>(j.at("user"), "flags", 0),Utils::value<int>(j.at("user"), "type", 0),
        Utils::value<long long>(j.at("user"), "created_at", 0));
}