#pragma once

#include <foxogram/EventTypes.h>
#include <foxogram/export.h>
#include <nlohmann/json.hpp>

#define event_decl(x) \
	class x : public Event { \
    protected:        \
        typedef void (*Lamda##x)( ::foxogram::x##_t );          \
        friend class foxogram::Gateway;                         \
        friend class foxogram::Me;              \
        Lamda##x callback = nullptr; \
    public:            \
        virtual void handle(Me* me, nlohmann::json j, const std::string raw); \
    };
namespace foxogram {
    class Me;
    class Gateway;
    namespace events {
        class LIBFOXOGRAM_EXPORT Event {
            virtual void handle(Me *me, nlohmann::json j, std::string raw) = 0;
        };
        event_decl(MessageCreate)
        event_decl(MessageUpdate)
        event_decl(MessageDelete)
        event_decl(ChannelCreate)
        event_decl(ChannelUpdate)
        event_decl(ChannelDelete)
        event_decl(MemberAdd)
        event_decl(MemberRemove)
        event_decl(MemberUpdate)
        event_decl(Hello)
        event_decl(Pong)

    }
}