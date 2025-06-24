#pragma once

#include <foxochat/EventTypes.h>
#include <foxochat/export.h>
#include <nlohmann/json.hpp>

#define event_decl(x) \
	class x : public Event { \
    protected:        \
        typedef void (*Lamda##x)( ::foxochat::x##_t );          \
        friend class foxochat::Gateway;                         \
        friend class foxochat::Me;              \
        Lamda##x callback = nullptr; \
    public:            \
        virtual void handle(Me* me, nlohmann::json eventJ, const std::string raw); \
    };
namespace foxochat {
    class Me;
    class Gateway;
    namespace events {
        class LIBFOXOCHAT_EXPORT Event {
        public:
            virtual void handle(Me *me, nlohmann::json eventJ, std::string raw) = 0;
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