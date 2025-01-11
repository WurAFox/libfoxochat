#pragma once

#include <foxogram/export.h>
#include <foxogram/Message.h>
#include <foxogram/Channel.h>
#include <foxogram/Member.h>
#include <foxogram/User.h>
#include <foxogram/Me.h>

namespace foxogram {
    struct LIBFOXOGRAM_EXPORT Event_t {
            public:
            std::string rawEvent = {};
            Me* me = nullptr;
            Event_t() = default;
            Event_t(const Event_t& e) = default;
            Event_t(Event_t&& e) = default;
            Event_t(Me* me, const std::string& raw) : me(me), rawEvent(raw) {};
            Event_t(Me* me, std::string&& raw) : me(me), rawEvent(raw) {};
            Event_t& operator=(const Event_t& e) = default;
            Event_t& operator=(Event_t&& e) = default;
            virtual ~Event_t() = 0;
    };

    struct LIBFOXOGRAM_EXPORT MessageCreate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Message* msg = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT MessageUpdate_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Message* msg = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT MessageDelete_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Message* msg = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT ChannelCreate : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Channel* channel = nullptr;
    };


    struct LIBFOXOGRAM_EXPORT ChannelUpdate : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Channel* channel = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT ChannelDelete_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Channel* channel = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT MemberAdd_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Member* member = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT MemberRemove_t : public Event_t {
        using Event_t::Event_t;
        using Event_t::operator=;
        Member* member = nullptr;
    };

    struct LIBFOXOGRAM_EXPORT MemberUpdate_t : public Event_t {

    };
}