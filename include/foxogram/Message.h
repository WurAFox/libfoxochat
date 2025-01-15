#pragma once

#include <foxogram/BaseEntity.h>
#include <string>
#include <list>

namespace foxogram {
    struct Channel;

    struct LIBFOXOGRAM_EXPORT Message : BaseEntity {
    private:
        std::shared_ptr<Channel> channel;
        long long authorId;
        long long timestamp;
        const long long id;
        std::list<std::string> attachments;
        std::string content;

    protected:
        std::string token;
        friend class Me;

        void handleError(const nlohmann::json &response) const override;

    public:
        Message(long long id, std::shared_ptr<Channel> channel, long long authorId, long long timestamp,
                std::string content, std::list<std::string> attachments);

        void deleteMessage() const;

        void edit() const;

        [[nodiscard]] std::string getIdOrName() const override;
    };
    using MessagePtr = std::shared_ptr<Message>;
}
