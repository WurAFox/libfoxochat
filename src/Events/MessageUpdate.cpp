#include <foxochat/Events.h>
#include <foxochat/Me.h>
#include <foxochat/Utils.h>

void foxochat::events::MessageUpdate::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    MessageUpdate_t e;
    e.rawEvent = raw;
    e.me = me;
    nlohmann::json j = eventJ.at("d");
    auto msg = foxochat::Message::fromJSON(j);
    msg->token = *me->token;
    auto channel = me->getChannel(Utils::value<long long>(j.at("channel"), "id", 0));
    if (!channel) {
        auto channelPtr = foxochat::Channel::fromJSON(j.at("channel"));
        channelPtr->token = *me->token;
        me->channels->store(channelPtr);
    }

    auto m = foxochat::Member::fromJSON(j.at("author"));
    auto member = channel->getMember(m->getId());
    msg->author = member ?: channel->members->store(m);
    e.msg = channel->messages->store(msg);
    if (callback != nullptr) {
        callback(e);
    }
}