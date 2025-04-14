#include <foxogram/Member.h>

foxogram::Member::Member(long long id, long long int channelId, long long permissions, const std::string &username,
                         const std::string &avatar, long long flags, int type, long long createdAt)
    : User(id, createdAt, username, avatar, flags, type),
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

std::shared_ptr<foxogram::Member> foxogram::Member::fromJSON(nlohmann::json j) {
    return std::make_shared<foxogram::Member>(j.value<long long>("id", 0),
        j.at("channel").value<long long>("id", 0),j.value<long long>("permission", 0),
        j.at("user").value<std::string>("username", ""),j.at("user").value<std::string>("avatar", ""),
        j.at("user").value<long long>("flags", 0),j.at("user").value<int>("type", 0),
        j.at("user").value<long long>("created_at", 0));
}