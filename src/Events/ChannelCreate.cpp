#include <foxochat/Events.h>
#include <foxochat/Me.h>

void foxochat::events::ChannelCreate::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    ChannelCreate_t e;
    e.me = me;
    e.rawEvent = raw;
    nlohmann::json j = eventJ.at("d");
    auto channel = foxochat::Channel::fromJSON(j);
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