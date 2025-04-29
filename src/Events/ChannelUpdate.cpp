#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::ChannelUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    ChannelUpdate_t e;
    e.me = me;
    e.rawEvent = raw;
    auto channel = foxogram::Channel::fromJSON(j);
    channel->token = *me->token;
    auto previousChannel = me->channels->get(channel->id);
    if (previousChannel != nullptr) {
        channel->members = previousChannel->members;
        channel->messages = previousChannel->messages;
    }
    e.channel = me->channels->store(channel);
    if (callback != nullptr) {
        callback(e);
    }
}