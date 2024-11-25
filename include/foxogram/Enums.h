#pragma once

#include <foxogram/export.h>

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT UserFlags {
    private:
        const long long bit;
    public:
        enum Flags {
                EMAIL_VERIFIED = 1,
                MFA_ENABLED = 1 << 1,
                DISABLED = 1 << 2,
        };
        explicit UserFlags(long long bit) : bit(bit) {}
        const long long getBit() const {
            return bit;
        }
    };

    class FOXOGRAM_LIB_EXPORT MemberPermissions {
    private:
        const long long bit;
    public:
        enum Permissions {
            ADMIN = 1,
            BAN_MEMBER = 1 << 1,
            KICK_MEMBER = 1 << 2,
            MANAGE_MESSAGES = 1 << 3,
            MANAGE_CHANNELS = 1 << 4,
            ATTACH_FILES = 1 << 5,
            SEND_MESSAGES = 1 << 6
        };
        explicit MemberPermissions(long long bit) : bit(bit) {}
        const long long getBit() const {
            return bit;
        };
    };

}