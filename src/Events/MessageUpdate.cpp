#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MessageUpdate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MessageUpdate_t e;
    e.rawEvent = raw;
    auto msg = foxogram::Message::fromJSON(j);
    msg->token = *me->token;
    e.msg = msg;
    if (callback != nullptr) {
        callback(e);
    }
}