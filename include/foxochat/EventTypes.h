#pragma once

#include <foxochat/export.h>
#include <foxochat/Message.h>
#include <foxochat/Channel.h>
#include <foxochat/Member.h>
#include <foxochat/User.h>

namespace foxochat {
    class Me;
    struct LIBFOXOCHAT_EXPORT Event_t {
            public:
            std::string rawEvent = {};
            Me* me = nullptr;
            Event_t() = default;
            Event_t(const Event_t& e) = default;
            Event_t(Event_t&& e) = default;
            Event_t(Me* me, const std::string& raw);
            Event_t(Me* me, std::string&& raw);
            Event_t& operator=(const Event_t& e) = default;
            Event_t& operator=(Event_t&& e) = default;
            virtual ~Event_t() = default;
    };

    struct LIBFOXOCHAT_EXPORT MessageCreate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MessagePtr msg = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT MessageUpdate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MessagePtr msg = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT MessageDelete_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MessagePtr msg = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT ChannelCreate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        ChannelPtr channel = nullptr;
    };


    struct LIBFOXOCHAT_EXPORT ChannelUpdate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        ChannelPtr channel = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT ChannelDelete_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        ChannelPtr channel = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT MemberAdd_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MemberPtr member = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT MemberRemove_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MemberPtr member = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT MemberUpdate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        MemberPtr member = nullptr;
    };

    struct LIBFOXOCHAT_EXPORT Hello_t : public Event_t {
        int interval = 0;
        using Event_t::Event_t;
        using Event_t::operator=;
    };

    struct LIBFOXOCHAT_EXPORT Pong_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
    };
}