#pragma once

#include <foxochat/export.h>
#include <foxochat/User.h>
#include <foxochat/Channel.h>
#include <foxochat/Cache.h>
#include <foxochat/Gateway.h>
#include <foxochat/Events.h>

#define decl_on_event(x) friend events::x; typedef void (*Lamda##x)( foxochat::x##_t );\
void on##x(Lamda##x callback) { \
    x->callback = callback;\
} 

namespace foxochat {
    class LIBFOXOCHAT_EXPORT Me : public User {
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
        friend events::Hello;
        events::Hello* Hello = new events::Hello();
        friend events::Pong;
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
        std::string* token;
        Gateway gateway;
        [[nodiscard]] User fetchMe(std::string* token);
        std::shared_ptr<Cache<Channel>> channels;
        std::shared_ptr<Cache<User>> userCache;
    public:
        explicit Me(const std::string& token);

        Me(const std::string& username, const std::string& email, const std::string& password);

        Me(const std::string& email, const std::string& password);

        [[nodiscard]] std::string getToken() const;

        [[nodiscard]] std::list<foxochat::ChannelPtr> getChannels() const;

        [[nodiscard]] std::list<foxochat::ChannelPtr> fetchChannels();

        Me(long long int id, std::string username, std::string avatar, long long int flags,
           int type, long long int createdAt) = delete;

        [[nodiscard]] UserPtr fetchUser(long long int id);

        [[nodiscard]] User fetchMe() const;

        static std::string login(std::string email, std::string password);

        static std::string signup(std::string username, std::string email, std::string password);

        bool verifyEmail(const std::string &code) const;

        bool resendEmail() const;

        bool deleteUser(std::string password) const;

        bool confirmDeleteUser(const std::string &code) const;

        ChannelPtr createChannel(std::string name, int type);

        ChannelPtr joinChannel(long long int id);

        [[nodiscard]] ChannelPtr fetchChannel(long long int id);

        [[nodiscard]] ChannelPtr getChannel(long long id);

        [[nodiscard]] UserPtr getUser(long long id);

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
