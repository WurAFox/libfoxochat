#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MemberAdd::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberAdd_t e;
    e.me = me;
    e.rawEvent = raw;
    auto member = std::make_shared<foxogram::Member>(j.at("id").get<long long>(),
        j.at("channel").at("id").get<long long>(),j.at("permission").get<long long>(),
        j.at("user").at("username").get<std::string>(),j.at("user").at("avatar").get<std::string>(),
        j.at("user").at("flags").get<long long>(),j.at("user").at("type").get<int>(),
        j.at("user").at("created_at").get<long long>());
    member->token = *me->token;
    e.member = member;
    if (callback != nullptr) {
        callback(e);
    }
}