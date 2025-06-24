#include <foxochat/Events.h>
#include <foxochat/Me.h>
#include <foxochat/Utils.h>

void foxochat::events::MemberUpdate::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    MemberUpdate_t e;
    e.me = me;
    e.rawEvent = raw;
    nlohmann::json j = eventJ.at("d");
    auto member = foxochat::Member::fromJSON(j);
    member->token = *me->token;
    auto channel = me->getChannel(Utils::value<long long>(j.at("channel"), "id", 0));
    if (!channel) {
        auto channelPtr = foxochat::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channels->store(channelPtr);
    }

    e.member = channel->members->store(member);
    if (callback != nullptr) {
        callback(e);
    }
}
