#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Channel.h>

namespace foxogram {
    class LIBFOXOGRAM_EXPORT Me : public User {
        std::string token;
        [[nodiscard]] User fetchMe(const std::string& token);
    public:
        explicit Me(const std::string& token);

        Me(std::string username, const std::string& email, const std::string& password);

        Me(const std::string& email, const std::string& password);

        Me(const Me &) = delete;

        Me(Me &&) = delete;

        Me(long long int id, std::string username, std::string avatar, long long int flags,
           int type, long long int createdAt) = delete;

        [[nodiscard]] User fetchUser(long long id) const;

        [[nodiscard]] User fetchMe() const;

        static std::string login(std::string email, std::string password);

        static std::string signup(std::string username, std::string email, std::string password);

        [[nodiscard]] bool verifyEmail(const std::string &code) const;

        void deleteUser(std::string password) const;

        [[nodiscard]] bool confirmDeleteUser(const std::string &code) const;

        [[nodiscard]] Channel createChannel(std::string name, int type) const;

        [[nodiscard]] Channel joinChannel(long long id) const;

        [[nodiscard]] Channel fetchChannel(long long id) const;

        [[nodiscard]] Message fetchMessage(long long id) const;

    protected:
        void handleError(const nlohmann::json &response) const override;
        static void _handleError(const nlohmann::json &response);
    };
}
