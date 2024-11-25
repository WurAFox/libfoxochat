#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Channel.h>

namespace foxogram {

    class FOXOGRAM_LIB_EXPORT Me : User {
    private:
        std::string token;
    public:
        explicit Me(std::string token);
        Me(std::string username, std::string email, std::string password);
        Me(std::string email, std::string password);
        Me(const Me&) = delete;
        Me(Me&&) = delete;
        Me(long long int id, std::string username, std::string avatar, long long int flags,
           int type, long long int createdAt) = delete;
        foxogram::User fetchUser(long long id);
        foxogram::User fetchMe();
        static std::string login(std::string email, std::string password);
        static std::string signup(std::string username, std::string email, std::string password);
        bool verifyEmail(std::string code);
        void deleteUser(std::string password);
        bool confirmDeleteUser(std::string code);
        Channel createChannel(std::string name, int type);
        Channel joinChannel(long long id);
        Channel fetchChannel(long long id);
        Message fetchMessage(long long id);
    };

}