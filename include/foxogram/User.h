#pragma once

#include <foxogram/export.h>
#include <foxogram/Enums.h>
#include <string>

namespace foxogram {
    struct FOXOGRAM_LIB_EXPORT User {
    protected:
        long long id;
        std::string username;
        std::string avatar;
        long long flags;
        int type;
        long long createdAt;
        User() = default;
    public:
        User(long long int id, std::string username, std::string avatar, long long int flags,
             int type, long long int createdAt);
        User(const User&) = default;
        User(User&&) = default;
        void addFlag(foxogram::UserFlags flag);
        void removeFlag(foxogram::UserFlags flag);
        bool hasFlag(foxogram::UserFlags flag);
        const std::string &getUsername() const;
        const std::string &getAvatar() const;
        long long int getFlags() const;
        int getType() const;
        long long int getCreatedAt() const;
        long long int getId() const;
    };
}