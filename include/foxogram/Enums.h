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
        UserFlags(long long bit) : bit(bit) {}
        const long long getBit() const {
            return bit;
        }
    };

}