#include <foxochat/User.h>
#include <foxochat/Utils.h>
#include <utility>

foxochat::User::User(long long id, long long createdAt, std::string username, Attachment avatar, long long int flags, int type, std::string displayName): createdAt(createdAt),
    username(std::move(username)), avatar(avatar), flags(flags), type(type),
    displayName(std::move(displayName)), BaseEntity(id) {}

void foxochat::User::handleError(const nlohmann::json &response) const {
}

void foxochat::User::addFlag(const UserFlags flag) {
    flags.addFlag(flag);
}

void foxochat::User::removeFlag(const UserFlags flag) {
    flags.removeFlag(flag);
}

bool foxochat::User::hasFlag(const UserFlags flag) const {
    return flags.hasFlag(flag);
}

const std::string &foxochat::User::getUsername() const {
    return username;
}

const foxochat::Attachment &foxochat::User::getAvatar() const {
    return avatar;
}

foxochat::FlagsBase<foxochat::UserFlags> foxochat::User::getFlags() const {
    return flags;
}

int foxochat::User::getType() const {
    return type;
}

long long int foxochat::User::getCreatedAt() const {
    return createdAt;
}

const std::string &foxochat::User::getDisplayName() const {
    return displayName;
}

std::shared_ptr<foxochat::User> foxochat::User::fromJSON(nlohmann::json j) {
    return std::make_shared<User>(
            Utils::value<long long>(j, "id", 0), Utils::value<long long>(j, "created_at", 0), Utils::value<std::string>(j, "username", ""),
             Utils::value<foxochat::Attachment>(j, "avatar", {0, "", "", "", 0}), Utils::value<long long>(j, "flags", 0),
            Utils::value<int>(j, "type", 0), j.at("display_name").is_string() ? Utils::value<std::string>(j, "display_name", "") : ""
    );
}