#pragma once

#include <foxogram/BaseEntity.h>
#include <string>
#include <list>

namespace foxogram {
    struct Channel;

    struct Message FOXOGRAM_LIB_EXPORT : BaseEntity {
    private:
        Channel *channel;
        long long authorId;
        long long timestamp;
        std::list<std::string> attachments;

    protected:
        std::string token;
        friend class Me;

        void handleError(const nlohmann::json &response) const override;

    public:
        Message(long long id, Channel *channel, long long authorId, long long timestamp,
                std::list<std::string> attachments);

        void deleteMessage() const;

        void edit() const;
    };
}
