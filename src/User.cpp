#include <foxogram/User.h>
#include <utility>

foxogram::User::User(long long id, long long createdAt, std::string username, std::string avatar, long long int flags, int type, std::string displayName): createdAt(createdAt),
    username(std::move(username)), avatar(std::move(avatar)), flags(flags), type(type),
    displayName(std::move(displayName)), BaseEntity(id) {}

void foxogram::User::handleError(const nlohmann::json &response) const {
}

void foxogram::User::addFlag(const UserFlags flag) {
    flags.addFlag(flag);
}

void foxogram::User::removeFlag(const UserFlags flag) {
    flags.removeFlag(flag);
}

bool foxogram::User::hasFlag(const UserFlags flag) const {
    return flags.hasFlag(flag);
}

const std::string &foxogram::User::getUsername() const {
    return username;
}

const std::string &foxogram::User::getAvatar() const {
    return avatar;
}

foxogram::FlagsBase<foxogram::UserFlags> foxogram::User::getFlags() const {
    return flags;
}

int foxogram::User::getType() const {
    return type;
}

long long int foxogram::User::getCreatedAt() const {
    return createdAt;
}

const std::string &foxogram::User::getDisplayName() const {
    return displayName;
}

foxogram::UserPtr foxogram::User::fromJSON(nlohmann::json j) {
    return std::make_shared<User>(
            j.at("id").get<long long>(), j.at("created_at").get<long long>(), j.at("username").get<std::string>(),
            j.at("avatar").is_string() ? j.at("avatar").get<std::string>() : "", j.at("flags").get<long long>(),
            j.at("type").get<int>(), j.at("created_at").is_string() ? j.at("created_at").get<std::string>() : ""
    );
}
