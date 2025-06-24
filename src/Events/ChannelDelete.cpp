#include <foxochat/Events.h>
#include <foxochat/Me.h>

void foxochat::events::ChannelDelete::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    ChannelDelete_t e;
    e.me = me;
    e.rawEvent = raw;
    nlohmann::json j = eventJ.at("d");
    auto channel = foxochat::Channel::fromJSON(j);
    channel->token = *me->token;
    e.channel = me->channels->get(channel->getId());
    me->channels->Delete(channel->getId());
    if (callback != nullptr) {
        callback(e);
    }
}