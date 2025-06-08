#pragma once

#include <bitset>

namespace LIBFOXOCHAT_EXPORT foxochat {
    enum LogLevel {
        LOG_DEBUG = 0,
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR,
        LOG_NOTHING,
    };

    template<typename EnumType>
    class LIBFOXOCHAT_EXPORT FlagsBase {
        long long bits;

    public:
        explicit constexpr FlagsBase(const long long bit = 0) : bits(bit) {
        }

        [[nodiscard]] constexpr long long toLongLong() const noexcept {
            return bits;
        }

        [[nodiscard]] constexpr bool hasFlag(EnumType flag) const noexcept {
            return (bits & static_cast<long long>(flag)) != 0;
        }

        void addFlag(EnumType flag) noexcept {
            setFlag(flag, true);
        }

        void removeFlag(EnumType flag) noexcept {
            setFlag(flag, false);
        }

    private:
        void setFlag(EnumType flag, const bool value = true) noexcept {
            if (value)
                bits |= static_cast<long long>(flag);
            else
                bits &= ~static_cast<long long>(flag);
        }
    };


    enum class UserFlags : long long {
        EMAIL_VERIFIED = 1,
        MFA_ENABLED = 1 << 1,
        DISABLED = 1 << 2,
    };

    using UserFlag = FlagsBase<UserFlags>;


    enum class MemberPermissions : long long {
        ADMIN = 1,
        BAN_MEMBER = 1 << 1,
        KICK_MEMBER = 1 << 2,
        MANAGE_MESSAGES = 1 << 3,
        MANAGE_CHANNELS = 1 << 4,
        ATTACH_FILES = 1 << 5,
        SEND_MESSAGES = 1 << 6,
    };

    using MemberPermission = FlagsBase<MemberPermissions>;

    enum class AttachmentFlags : long long {
        SPOILER = 1,
    };

    using AttachmentFlag = FlagsBase<AttachmentFlags>;
}
