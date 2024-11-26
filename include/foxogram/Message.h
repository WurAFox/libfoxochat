#pragma once

#include <foxogram/export.h>
#include <string>
#include <list>

namespace foxogram {

    struct Channel;

    struct FOXOGRAM_LIB_EXPORT Message {
    private:
        long long id;
        foxogram::Channel* channel;
        long long authorId;
        long long timestamp;
        std::list<std::string> attachments;
    protected:
        std::string token;
        friend class Me;
    public:
        Message(long long id, foxogram::Channel* channel, long long authorId, long long timestamp, std::list<std::string> attachments);
        void deleteMessage();
        void edit();

    };

}
