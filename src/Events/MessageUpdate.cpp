#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MessageUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MessageUpdate_t e;
    e.rawEvent = raw;
    auto msg = std::make_shared<foxogram::Message>(j.at("id").get<long long>(), j.at("channel").at("id").get<long long>(),
        j.at("author").at("id").get<long long>(), j.at("created_at").get<long long>(),
        j.at("content").get<std::string>(), j.at("attachments").get<std::list<std::string>>());
    msg->token = *me->token;
    e.msg = msg;
    if (callback != nullptr) {
        callback(e);
    }
}