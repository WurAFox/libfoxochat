#pragma once

#include <foxogram/export.h>
#include <string>
#include <list>
#include <foxogram/Member.h>
#include <foxogram/Message.h>
#include <foxogram/BaseEntity.h>

namespace foxogram {
    struct LIBFOXOGRAM_EXPORT Channel : BaseEntity {
        friend class Me;

    private:
        std::string name;
        const int type;
        long long ownerId;
        std::list<Member> members;
        std::list<Message> messages;
        const long long id;
        const long long createdAt;

    protected:
        std::string token;

        void handleError(const nlohmann::json &response) const override;

    public:
        Channel(long long id, std::string name, int type, long long int ownerId);

        void leave() const;

        void edit() const;

        void deleteChannel() const;

        [[nodiscard]] std::list<Message> getMessages() const;

        [[nodiscard]] Message getMessage(long long id) const;

        [[nodiscard]] Message createMessage() const;

        [[nodiscard]] const std::string &getName() const;

        [[nodiscard]] int getType() const;

        [[nodiscard]] long long getId() const;

        [[nodiscard]] long long int getOwnerId() const;

        [[nodiscard]] const std::list<Member> &getMembers() const;
    };
}
