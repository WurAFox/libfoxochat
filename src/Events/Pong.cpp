#include <foxogram/Events.h>
#include <foxogram/Me.h>

void foxogram::events::Pong::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    Pong_t e;
    e.me = me;
    e.rawEvent = raw;
    if (callback != nullptr) {
        callback(e);
    }
}