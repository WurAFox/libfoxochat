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
    return std::make_shared<foxogram::Member>(j.at("id").get<long long>(),
        j.at("channel").at("id").get<long long>(),j.at("permission").get<long long>(),
        j.at("user").at("username").get<std::string>(),j.at("user").at("avatar").get<std::string>(),
        j.at("user").at("flags").get<long long>(),j.at("user").at("type").get<int>(),
        j.at("user").at("created_at").get<long long>());
}