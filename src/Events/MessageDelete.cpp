#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MessageDelete::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MessageDelete_t e;
    e.me = me;
    e.rawEvent = raw;
    auto msg = foxogram::Message::fromJSON(j);
    msg->token = *me->token;
    auto channel = me->getChannel(j.at("channel").at("id").get<long long>());
    if (!channel) {
        auto channelPtr = foxogram::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channelCache->store(channelPtr);
    }
    auto m = foxogram::Member::fromJSON(j.at("author"));
    auto member = channel->getMember(m->getId());
    msg->author = member ?: channel->members->store(m);

    e.msg = channel->messages->get(msg->id);
    if (callback != nullptr) {
        callback(e);
    }
}