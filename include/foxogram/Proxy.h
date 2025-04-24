#pragma once
#include <foxogram/export.h>
#include <foxogram/Enums.h>
#include <memory>
#include <list>

namespace foxogram {
    namespace events {
        class ChannelCreate;
        class ChannelDelete;
        class ChannelUpdate;
        class Hello;
        class MemberAdd;
        class MemberRemove;
        class MemberUpdate;
        class MessageCreate;
        class MessageDelete;
        class MessageUpdate;
        class Pong;
    }

    template<typename T>
    class LIBFOXOGRAM_EXPORT Proxy {
    };

    using MessagePtr = std::shared_ptr<Proxy<class Message>>;
    using MemberPtr = std::shared_ptr<Proxy<class Member>>;
    using ChannelPtr = std::shared_ptr<Proxy<class Channel>>;
    using UserPtr = std::shared_ptr<Proxy<class User>>;


    template<typename T>
    class Cache;
    template<>
    class LIBFOXOGRAM_EXPORT Proxy<class User> {
    protected:
        std::shared_ptr<User> ptr;
        friend Cache<User>;
    public:
        [[nodiscard]] const std::string &getDisplayName() const;

        void addFlag(UserFlags flag);

        void removeFlag(UserFlags flag);

        [[nodiscard]] bool hasFlag(UserFlags flag) const;

        [[nodiscard]] const std::string &getUsername() const;

        [[nodiscard]] const std::string &getAvatar() const;

        [[nodiscard]] FlagsBase<UserFlags> getFlags() const;

        [[nodiscard]] int getType() const;

        [[nodiscard]] long long getCreatedAt() const;
    };

    template<>
    class LIBFOXOGRAM_EXPORT Proxy<class Member> {
    protected:
        std::shared_ptr<Member> ptr;
        friend Cache<Member>;
    public:
        [[nodiscard]] long long int getChannelId() const;

        [[nodiscard]] const FlagsBase<MemberPermissions> &getPermissions() const;

        void addPermissions(MemberPermissions permission);

        void removePermissions(MemberPermissions permission);

        [[nodiscard]] bool hasPermissions(MemberPermissions permission) const;
    };

    template<>
    class LIBFOXOGRAM_EXPORT Proxy<class Channel> {
    protected:
        std::shared_ptr<Cache<Message>> messages;
        std::shared_ptr<Cache<Member>> members;
        std::shared_ptr<Channel> ptr;
        friend Cache<Channel>;
        friend events::ChannelCreate;
        friend events::ChannelUpdate;
        friend events::ChannelDelete;
        friend events::MessageUpdate;
        friend events::MessageCreate;
        friend events::MessageDelete;
        friend events::MemberAdd;
        friend events::MemberUpdate;
        friend events::MemberRemove;
    public:
        [[nodiscard]] long long int getCreatedAt() const;

        [[nodiscard]] const std::string &getDisplayName() const;

        [[nodiscard]] const std::string &getIcon() const;

        bool leave();

        void edit(const std::string& displayName = "", const std::string& name = "", const std::string& icon = "");

        bool deleteChannel();

        [[nodiscard]] std::list<MessagePtr> getMessages() const;

        std::list<MessagePtr> fetchMessages(long long before = 0, int limit= 25);

        [[nodiscard]] MessagePtr fetchMessage(long long id);

        [[nodiscard]] MessagePtr createMessage(std::string content, const std::list<std::string>& attachments = {});

        [[nodiscard]] std::string getName() const;

        [[nodiscard]] int getType() const;

        [[nodiscard]] std::string getOwnerName() const;

        [[nodiscard]] std::list<MemberPtr> getMembers() const;

        std::list<MemberPtr> fetchMembers();

        MemberPtr fetchMember(long long id);

        MemberPtr getMember(long long id);
    };

    template<>
    class LIBFOXOGRAM_EXPORT Proxy<class Message> {
    protected:
        std::shared_ptr<Message> ptr;
        friend Cache<Message>;
    public:
        void deleteMessage() const;

        void edit() const;
        [[nodiscard]] long long int getChannelId() const;

        [[nodiscard]] long long int getAuthorId() const;

        [[nodiscard]] long long int getCreatedAt() const;

        [[nodiscard]] const std::list<std::string> &getAttachments() const;

        [[nodiscard]] const std::string &getContent() const;

        [[nodiscard]] std::shared_ptr<Proxy<Member>> getAuthor() const;
    };
}