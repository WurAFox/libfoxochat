#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::ChannelCreate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    ChannelCreate_t e;
    e.me = me;
    e.rawEvent = raw;
    auto channel = foxogram::Channel::fromJSON(j);
    channel->token = *me->token;
    e.channel = channel;
    if (callback != nullptr) {
        callback(e);
    }
}