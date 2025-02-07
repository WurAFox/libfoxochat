#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::MessageCreate::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    MessageCreate_t e;
    e.me = me;
    e.rawEvent = raw;
    auto msg = foxogram::Message::fromJSON(j);
    msg->token = *me->token;
    e.msg = msg;
    if (callback != nullptr) {
        callback(e);
    }
}