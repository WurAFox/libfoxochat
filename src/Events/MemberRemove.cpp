#include <foxogram/Events.h>
#include <foxogram/Me.h>
#include <foxogram/Utils.h>

void foxogram::events::MemberRemove::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MemberRemove_t e;
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

    e.member = channel->members->get(member->id);
    if (callback != nullptr) {
        callback(e);
    }
}