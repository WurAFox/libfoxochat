#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Enums.h>

namespace foxogram {

    struct Channel;

    struct FOXOGRAM_LIB_EXPORT Member : foxogram::User {
    private:
        foxogram::Channel* channel;
        long long permissions;
    public:
        void addPermissions(MemberPermissions permission);
        void removePermissions(MemberPermissions permission);
        bool hasPermissions(MemberPermissions permission);
        Member(long long id, Channel* channel, long long permission, std::string avatar, long long createdAt, long long flags, int type);

    };

}