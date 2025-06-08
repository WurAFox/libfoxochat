#pragma once

#include <nlohmann/json.hpp>

namespace foxochat {
    class User;
    class Attachment;
    class Message;
    class Member;
    class Channel;
}

namespace nlohmann {
    template <>
    struct adl_serializer<foxochat::User> {
        static foxochat::User from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxochat::User u);
    };
    
    template <>
    struct nlohmann::adl_serializer<foxochat::Attachment> {
        static foxochat::Attachment from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxochat::Attachment a);
    };

    template <>
    struct nlohmann::adl_serializer<foxochat::Member> {
        static foxochat::Member from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxochat::Member a);
    };

    template <>
    struct nlohmann::adl_serializer<foxochat::Channel> {
        static foxochat::Channel from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxochat::Channel a);
    };

    template <>
    struct nlohmann::adl_serializer<foxochat::Message> {
        static foxochat::Message from_json(const nlohmann::json &j);

        static void to_json(nlohmann::json &j, foxochat::Message a);
    };
}
