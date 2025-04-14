#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MemberRemove::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberRemove_t e;
    e.me = me;
    e.rawEvent = raw;
    auto member = foxogram::Member::fromJSON(j);
    member->token = *me->token;
    auto channel = me->getChannel(j.at("channel").value<long long>("id", 0));
    if (!channel) {
        auto channelPtr = foxogram::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channelCache->store(channelPtr);
    }

    e.member = channel->members->get(member->id);
    if (callback != nullptr) {
        callback(e);
    }
}