#include <foxogram/User.h>
#include <utility>

foxogram::User::User(long long int id, std::string username, std::string avatar,
                             long long int flags, int type, long long int createdAt) : id(id), username(std::move(username)),
                                                                                       avatar(std::move(avatar)), flags(flags),
                                                                                       type(type),
                                                                                       createdAt(createdAt) {}

void foxogram::User::addFlag(foxogram::UserFlags flag) {
    this->flags |= flag.getBit();
}

void foxogram::User::removeFlag(foxogram::UserFlags flag) {
    this->flags &= flag.getBit();
}

bool foxogram::User::hasFlag(foxogram::UserFlags flag) {
    return (this->flags & flag.getBit()) != 0;
}

const std::string &foxogram::User::getUsername() const {
    return username;
}

const std::string &foxogram::User::getAvatar() const {
    return avatar;
}

long long int foxogram::User::getFlags() const {
    return flags;
}

int foxogram::User::getType() const {
    return type;
}

long long int foxogram::User::getCreatedAt() const {
    return createdAt;
}

long long int foxogram::User::getId() const {
    return id;
}
