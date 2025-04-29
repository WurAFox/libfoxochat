#include <foxogram/Events.h>
#include <foxogram/Me.h>
#include <foxogram/Utils.h>

void foxogram::events::MessageUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MessageUpdate_t e;
    e.rawEvent = raw;
    auto msg = foxogram::Message::fromJSON(j);
    msg->token = *me->token;
    auto channel = me->getChannel(Utils::value<long long>(j.at("channel"), "id", 0));
    if (!channel) {
        auto channelPtr = foxogram::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channels->store(channelPtr);
    }

    auto m = foxogram::Member::fromJSON(j.at("author"));
    auto member = channel->getMember(m->getId());
    msg->author = member ?: channel->members->store(m);
    e.msg = channel->messages->store(msg);
    if (callback != nullptr) {
        callback(e);
    }
}