#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MemberUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberUpdate_t e;
    e.me = me;
    e.rawEvent = raw;
    auto member = foxogram::Member::fromJSON(j);
    member->token = *me->token;
    e.member = member;
    if (callback != nullptr) {
        callback(e);
    }
}