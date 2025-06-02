#include <foxogram/Member.h>
#include <foxogram/Utils.h>

#include <utility>

foxogram::Member::Member(long long id, long long int channelId, long long permissions, const std::string &username,
                         Attachment avatar, long long flags, int type, long long createdAt)
    : User(id, createdAt, username, std::move(avatar), flags, type),
      channelId(channelId), permissions(permissions) {
}

void foxogram::Member::addPermissions(const MemberPermissions permission) {
    permissions.addFlag(permission);
}

bool foxogram::Member::hasPermissions(const MemberPermissions permission) const {
    return permissions.hasFlag(permission);
}

void foxogram::Member::removePermissions(const MemberPermissions permission) {
    permissions.removeFlag(permission);
}

long long int foxogram::Member::getChannelId() const {
    return channelId;
}

const foxogram::FlagsBase<foxogram::MemberPermissions> &foxogram::Member::getPermissions() const {
    return permissions;
}

std::shared_ptr<foxogram::Member> foxogram::Member::fromJSON(nlohmann::json j, long long channelId) {
    return std::make_shared<foxogram::Member>(Utils::value<long long>(j, "id", 0),
        j.contains("channel") ? Utils::value<long long>(j.at("channel"), "id", 0) : channelId,
        Utils::value<long long>(j, "permissions", 0), Utils::value<std::string>(j.at("user"), "username", ""),
        Utils::value<Attachment>(j.at("user"), "avatar", {0, "", "", "", 0}),
        Utils::value<long long>(j.at("user"), "flags", 0),Utils::value<int>(j.at("user"), "type", 0),
        Utils::value<long long>(j.at("user"), "created_at", 0));
}