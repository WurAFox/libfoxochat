#pragma once

#include <foxogram/BaseEntity.h>
#include <string>
#include <list>

namespace foxogram {
    struct Channel;

    struct LIBFOXOGRAM_EXPORT Message : BaseEntity {
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
