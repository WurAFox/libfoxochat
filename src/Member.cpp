#include <foxogram/Member.h>

#include <utility>

void foxogram::Member::addPermissions(foxogram::MemberPermissions permission) {
    this->permissions |= permission.getBit();
}

bool foxogram::Member::hasPermissions(foxogram::MemberPermissions permission) {
    return (this->permissions & permission.getBit()) != 0;
}

void foxogram::Member::removePermissions(foxogram::MemberPermissions permission) {
    this->permissions &= permission.getBit();
}

foxogram::Member::Member(long long int id, foxogram::Channel* channel, long long int permission, std::string avatar,
                         long long int createdAt, long long int flags, int type) {
    this->id = id;
    this->channel = channel;
    this->permissions = permission;
    this->avatar = std::move(avatar);
    this->createdAt = createdAt;
    this->flags = flags;
    this->type = type;
}
