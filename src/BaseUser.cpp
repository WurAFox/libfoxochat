#include <foxogram/BaseUser.h>
#include <utility>

foxogram::BaseUser::BaseUser(long long int id, std::string username, std::string avatar,
                             long long int flags, int type, long long int createdAt) : id(id), username(std::move(username)),
                                                                                       avatar(std::move(avatar)), flags(flags),
                                                                                       type(type),
                                                                                       createdAt(createdAt) {}

void foxogram::BaseUser::addFlag(foxogram::UserFlags flag) {
    this->flags |= flag.getBit();
}

void foxogram::BaseUser::removeFlag(foxogram::UserFlags flag) {
    this->flags &= flag.getBit();
}

bool foxogram::BaseUser::hasFlag(foxogram::UserFlags flag) {
    return (this->flags & flag.getBit()) != 0;
}