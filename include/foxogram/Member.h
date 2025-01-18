#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Enums.h>

namespace foxogram {
    struct Channel;

    struct LIBFOXOGRAM_EXPORT Member : User {
        friend class Me;

    private:
        long long channelId;

        FlagsBase<MemberPermissions> permissions;

    protected:
        std::string token;

    public:
        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;

        Member(long long id, long long channelId, long long permissions, const std::string &username,
               const std::string &avatar, long long flags, int type, long long createdAt);

    };

    using MemberPtr = std::shared_ptr<Member>;
}
