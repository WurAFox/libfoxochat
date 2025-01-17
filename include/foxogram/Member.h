#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Enums.h>

namespace foxogram {
    struct Channel;

    struct LIBFOXOGRAM_EXPORT Member : User {
        friend class Me;

    private:
        Channel *channel;

        FlagsBase<MemberPermissions> permissions;

    protected:
        std::string token;

    public:
        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;

        Member(long long id, Channel *channel, long long permissions, const std::string &username,
               const std::string &avatar, long long flags, int type, long long createdAt);

    };
}
