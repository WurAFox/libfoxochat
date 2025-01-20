#pragma once

#include <foxogram/BaseEntity.h>
#include <string>
#include <list>

namespace foxogram {
    struct Channel;
    namespace events {
        class MessageCreate;
        class MessageUpdate;
        class MessageDelete;
    }
    struct LIBFOXOGRAM_EXPORT Message : BaseEntity {
        friend events::MessageCreate;
        friend events::MessageUpdate;
        friend events::MessageDelete;
    private:
        long long channelId;
        long long authorId;
        long long timestamp;
        std::list<std::string> attachments;
        std::string content;

    protected:
        std::string token;
        friend class Me;

        void handleError(const nlohmann::json &response) const override;

    public:
        Message(long long id, long long int channelId, long long authorId, long long timestamp,
                std::string content, std::list<std::string> attachments);

        void deleteMessage() const;

        void edit() const;

    };
    using MessagePtr = std::shared_ptr<Message>;
}
