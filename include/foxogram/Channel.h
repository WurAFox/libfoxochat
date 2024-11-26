#pragma once

#include <foxogram/export.h>
#include <string>
#include <list>
#include <foxogram/Member.h>
#include <foxogram/Message.h>

namespace foxogram {


    struct FOXOGRAM_LIB_EXPORT Channel {
        friend class Me;
    private:
        long long id;
        std::string name;
        int type;
        long long ownerId;
        std::list<foxogram::Member> members;
        std::list<foxogram::Message> messages;
    protected:
        std::string token;
    public:
        Channel(long long id, std::string name, int type, long long int ownerId);
        void leave();
        void edit();
        void deleteChannel();
        std::list<foxogram::Message> getMessages();
        foxogram::Message getMessage(long long id);
        foxogram::Message createMessage();
        [[nodiscard]] long long int getId() const;
        [[nodiscard]] const std::string &getName() const;
        [[nodiscard]] int getType() const;
        [[nodiscard]] long long int getOwnerId() const;
        [[nodiscard]] const std::list<foxogram::Member> &getMembers() const;

    };

}