#pragma once

#include <nlohmann/json.hpp>

namespace foxogram {
    class User;
    class Attachment;
    class Message;
    class Member;
    class Channel;
}

namespace nlohmann {
    template <>
    struct adl_serializer<foxogram::User> {
        static foxogram::User from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxogram::User u);
    };
    
    template <>
    struct nlohmann::adl_serializer<foxogram::Attachment> {
        static foxogram::Attachment from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxogram::Attachment a);
    };

    template <>
    struct nlohmann::adl_serializer<foxogram::Member> {
        static foxogram::Member from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxogram::Member a);
    };

    template <>
    struct nlohmann::adl_serializer<foxogram::Channel> {
        static foxogram::Channel from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxogram::Channel a);
    };

    template <>
    struct nlohmann::adl_serializer<foxogram::Message> {
        static foxogram::Message from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxogram::Message a);
    };
}
