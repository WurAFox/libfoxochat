#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Enums.h>

namespace foxogram {
    namespace events {
        class MemberAdd;
        class MemberUpdate;
        class MemberRemove;
    }
    struct LIBFOXOGRAM_EXPORT Member : User {
        friend class Me;
        friend events::MemberAdd;
        friend events::MemberUpdate;
        friend events::MemberRemove;
    private:
        long long channelId;

        FlagsBase<MemberPermissions> permissions;

    protected:
        std::string token;

    public:
        [[nodiscard]] static std::shared_ptr<Member> fromJSON(nlohmann::json j);

        [[nodiscard]] long long int getChannelId() const;

        [[nodiscard]] const FlagsBase<MemberPermissions> &getPermissions() const;

        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;

        Member(long long id, long long channelId, long long permissions, const std::string &username,
               const std::string &avatar, long long flags, int type, long long createdAt);

    };

    using MemberPtr = std::shared_ptr<Member>;
}
