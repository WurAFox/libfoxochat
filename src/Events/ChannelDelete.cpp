#include <foxochat/Events.h>
#include <foxochat/Me.h>

void foxochat::events::ChannelDelete::handle(foxochat::Me *me, nlohmann::json j, const std::string raw) {
    ChannelDelete_t e;
    e.me = me;
    e.rawEvent = raw;
    auto channel = foxochat::Channel::fromJSON(j);
    channel->token = *me->token;
    e.channel = me->channels->get(channel->getId());
    me->channels->Delete(channel->getId());
    if (callback != nullptr) {
        callback(e);
    }
}