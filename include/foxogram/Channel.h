#pragma once

#include <foxogram/export.h>
#include <string>
#include <map>
#include <foxogram/Member.h>
#include <foxogram/Message.h>
#include <foxogram/Message.h>
#include <foxogram/BaseEntity.h>
#include <foxogram/Cache.h>

namespace foxogram {
    namespace events {
        class ChannelCreate;
        class ChannelUpdate;
        class ChannelDelete;
        class MessageUpdate;
        class MessageCreate;
        class MessageDelete;
        class MemberAdd;
        class MemberRemove;
        class MemberUpdate;
    }
    class Me;
    struct LIBFOXOGRAM_EXPORT Channel : BaseEntity {
        friend class Me;
        friend events::ChannelCreate;
        friend events::ChannelUpdate;
        friend events::ChannelDelete;
        friend events::MessageUpdate;
        friend events::MessageCreate;
        friend events::MessageDelete;
        friend events::MemberAdd;
        friend events::MemberRemove;
        friend events::MemberUpdate;
    private:
        std::string name;
        const int type;
        std::string ownerName;
        const long long createdAt;
        std::string displayName;
        std::string icon;

    protected:
        std::shared_ptr<Cache<Member>> members;
        std::shared_ptr<Cache<Message>> messages;
        Me* me;
        std::string token;

        void handleError(const nlohmann::json &response) const override;

    public:
        [[nodiscard]] static std::shared_ptr<Channel> fromJSON(nlohmann::json j);

        [[nodiscard]] long long int getCreatedAt() const;

        [[nodiscard]] const std::string &getDisplayName() const;

        [[nodiscard]] const std::string &getIcon() const;

        Channel(long long id, std::string name, std::string displayName, short type, std::string ownerName, long long createdAt, std::string icon);

        bool leave();

        void edit(const std::string& displayName = "", const std::string& name = "", const std::string& icon = "");

        bool deleteChannel();

        [[nodiscard]] std::list<MessagePtr> getMessages() const;

        std::list<MessagePtr> fetchMessages();

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
}
