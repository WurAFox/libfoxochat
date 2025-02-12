#pragma once

#include <foxogram/export.h>
#include <foxogram/Enums.h>
#include <foxogram/BaseEntity.h>
#include <string>

namespace foxogram {
    class Me;
    struct LIBFOXOGRAM_EXPORT User : BaseEntity {
    protected:
        std::string username;
        std::string avatar;
        FlagsBase<UserFlags> flags;
        int type{};
        long long createdAt{};
        std::string displayName;
        void handleError(const nlohmann::json &response) const override;
    public:
        [[nodiscard]] static std::shared_ptr<User> fromJSON(nlohmann::json j);

        [[nodiscard]] const std::string &getDisplayName() const;

        User(long long id, long long createdAt, std::string username, std::string avatar, long long int flags, int type, std::string displayName = "");

        User(const User &) = default;

        User(User &&) = default;

        void addFlag(UserFlags flag);

        void removeFlag(UserFlags flag);

        [[nodiscard]] bool hasFlag(UserFlags flag) const;

        [[nodiscard]] const std::string &getUsername() const;

        [[nodiscard]] const std::string &getAvatar() const;

        [[nodiscard]] FlagsBase<UserFlags> getFlags() const;

        [[nodiscard]] int getType() const;

        [[nodiscard]] long long getCreatedAt() const;

    };
    using UserPtr = std::shared_ptr<User>;
}
