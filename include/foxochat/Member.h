#pragma once

#include <foxochat/export.h>
#include <foxochat/User.h>
#include <foxochat/Enums.h>
#include <foxochat/Cache.h>
#include <foxochat/Serializer.h>

namespace foxochat {
    namespace events {
        class MemberAdd;
        class MemberUpdate;
        class MemberRemove;
    }
    class Me;
    class Channel;

    using MemberPtr = std::shared_ptr<foxochat::Member>;

    struct LIBFOXOCHAT_EXPORT Member : public User {
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
        [[nodiscard]] static std::shared_ptr<Member> fromJSON(nlohmann::json j, long long channelId = 0);

        [[nodiscard]] long long int getChannelId() const;

        [[nodiscard]] const FlagsBase<MemberPermissions> &getPermissions() const;

        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;

        Member(long long id, long long channelId, long long permissions, const std::string &username,
               Attachment avatar, long long flags, int type, long long createdAt);

    };

}
