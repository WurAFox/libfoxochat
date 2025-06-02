#pragma once

#include <foxogram/BaseEntity.h>
#include <foxogram/Cache.h>
#include <string>
#include <list>
#include <foxogram/Serializer.h>
#include <foxogram/Attachment.h>

namespace foxogram {
    struct Channel;
    namespace events {
        class MessageCreate;
        class MessageUpdate;
        class MessageDelete;
    }
    class Member;
    class Me;
    struct LIBFOXOGRAM_EXPORT Message : BaseEntity {
        friend events::MessageCreate;
        friend events::MessageUpdate;
        friend events::MessageDelete;
    private:
        long long channelId;
        long long authorId;
        long long timestamp;
        std::list<Attachment> attachments;
        std::string content;

    protected:
        std::string token;
        friend class Me;
        friend class Channel;
        std::shared_ptr<Proxy<Member>> author;
        void handleError(const nlohmann::json &response) const override;

    public:
        [[nodiscard]] static std::shared_ptr<Message> fromJSON(nlohmann::json j);

        Message(long long id, long long int channelId, long long authorId, long long timestamp,
                std::string content, std::list<Attachment> attachments);

        void deleteMessage() const;

        void edit() const;
        [[nodiscard]] long long int getChannelId() const;

        [[nodiscard]] long long int getAuthorId() const;

        [[nodiscard]] long long int getCreatedAt() const;

        [[nodiscard]] const std::list<Attachment> &getAttachments() const;

        [[nodiscard]] const std::string &getContent() const;

        [[nodiscard]] std::shared_ptr<Proxy<Member>> getAuthor() const;

    };
}
