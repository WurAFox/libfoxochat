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
        void leave(long long id);
        void edit(long long id);
        void deleteChannel(long long id);
    };

}