#include <foxogram/Events.h>
#include <foxogram/Me.h>
#include <foxogram/Utils.h>

void foxogram::events::MemberUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberUpdate_t e;
    e.me = me;
    e.rawEvent = raw;
    auto member = foxogram::Member::fromJSON(j);
    member->token = *me->token;
    auto channel = me->getChannel(Utils::value<long long>(j.at("channel"), "id", 0));
    if (!channel) {
        auto channelPtr = foxogram::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channels->store(channelPtr);
    }

    e.member = channel->members->store(member);
    if (callback != nullptr) {
        callback(e);
    }
}
