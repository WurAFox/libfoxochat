#pragma once

#include <foxogram/export.h>
#include <foxogram/User.h>
#include <foxogram/Channel.h>
#include <foxogram/Cache.h>
#include <foxogram/Gateway.h>
#include <foxogram/Events.h>

#define decl_on_event(x) typedef void (*Lamda##x)( foxogram::x##_t );\
void on##x(Lamda##x callback) { \
    x->callback = callback;\
} 

namespace foxogram {
    class LIBFOXOGRAM_EXPORT Me : public User {
        friend Gateway;
    protected:
        events::MessageCreate* MessageCreate = new events::MessageCreate();
        events::MessageUpdate* MessageUpdate = new events::MessageUpdate();
        events::MessageDelete* MessageDelete = new events::MessageDelete();
        events::ChannelCreate* ChannelCreate = new events::ChannelCreate();
        events::ChannelDelete* ChannelDelete = new events::ChannelDelete();
        events::ChannelUpdate* ChannelUpdate = new events::ChannelUpdate();
        events::MemberAdd* MemberAdd = new events::MemberAdd();
        events::MemberRemove* MemberRemove = new events::MemberRemove();
        events::MemberUpdate* MemberUpdate = new events::MemberUpdate();
        events::Hello* Hello = new events::Hello();
        events::Pong* Pong = new events::Pong();
        std::map<std::string, events::Event*> eventMap {
                {"MESSAGE_CREATE", MessageCreate},
                {"MESSAGE_UPDATE", MessageUpdate},
                {"MESSAGE_DELETE", MessageUpdate},
                {"CHANNEL_CREATE", MessageCreate},
                {"CHANNEL_UPDATE", ChannelUpdate},
                {"CHANNEL_DELETE", ChannelDelete},
                {"MEMBER_ADD", MemberAdd},
                {"MEMBER_REMOVE", MemberRemove},
                {"MEMBER_UPDATE", MemberUpdate},
                {"HELLO", Hello},
                {"PONG", Pong}
        };
        std::shared_ptr<Cache<User>> userCache = std::make_shared<Cache<User>>();
        std::shared_ptr<Cache<Channel>> channelCache = std::make_shared<Cache<Channel>>();
        std::string* token;
        Gateway* gateway;
        [[nodiscard]] User fetchMe(std::string* token);
    public:
        explicit Me(const std::string& token);

        Me(const std::string& username, const std::string& email, const std::string& password);

        Me(const std::string& email, const std::string& password);

        Me(const Me &) = delete;

        Me(Me &&) = delete;

        Me(long long int id, std::string username, std::string avatar, long long int flags,
           int type, long long int createdAt) = delete;

        [[nodiscard]] UserPtr fetchUser(const std::string& username);

        [[nodiscard]] UserPtr getUser(const std::string& username);

        [[nodiscard]] UserPtr getOrFetchUser(const std::string& username);

        [[nodiscard]] User fetchMe() const;

        static std::string login(std::string email, std::string password);

        static std::string signup(std::string username, std::string email, std::string password);

        bool verifyEmail(const std::string &code) const;

        bool resendEmail() const;

        bool deleteUser(std::string password) const;

        bool confirmDeleteUser(const std::string &code) const;

        ChannelPtr createChannel(std::string name, int type);

        ChannelPtr joinChannel(std::string name);

        [[nodiscard]] ChannelPtr fetchChannel(std::string name);

        [[nodiscard]] ChannelPtr getChannel(std::string name);

        [[nodiscard]] ChannelPtr getOrFetchChannel(const std::string &name);


        ~Me() override;

        decl_on_event(MessageCreate)
        decl_on_event(MessageUpdate)
        decl_on_event(MessageDelete)
        decl_on_event(ChannelCreate)
        decl_on_event(ChannelUpdate)
        decl_on_event(ChannelDelete)
        decl_on_event(MemberAdd)
        decl_on_event(MemberRemove)
        decl_on_event(MemberUpdate)
        decl_on_event(Hello)
        decl_on_event(Pong)

    protected:
        void handleError(const nlohmann::json &response) const override;
        static void _handleError(const nlohmann::json &response);
    };
}
