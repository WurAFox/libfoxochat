#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::ChannelDelete::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    ChannelDelete_t e;
    e.me = me;
    e.rawEvent = raw;
    auto channel = std::make_shared<Channel>(j.at("id").get<long long>(),
        j.at("name").get<std::string>(), j.at("display_name").get<std::string>(), j.at("type").get<int>(),
        j.at("owner").get<std::string>(), j.at("created_at").get<long long>(), j.at("icon").get<std::string>());
    channel->token = *me->token;
    e.channel = channel;
    if (callback != nullptr) {
        callback(e);
    }
}