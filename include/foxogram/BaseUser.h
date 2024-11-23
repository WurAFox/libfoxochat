#pragma once

#include <foxogram/export.h>
#include <foxogram/Enums.h>
#include <string>

namespace foxogram {
    class FOXOGRAM_LIB_EXPORT BaseUser {
    protected:
        long long id;
        std::string username;
        std::string avatar;
        long long flags;
        int type;
        long long createdAt;
    public:
        BaseUser(long long int id, std::string username, std::string avatar, long long int flags,
                 int type, long long int createdAt);
        void addFlag(foxogram::UserFlags flag);
        void removeFlag(foxogram::UserFlags flag);
        bool hasFlag(foxogram::UserFlags flag);
    };
}