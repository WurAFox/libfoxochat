#include <foxochat/Events.h>
#include <foxochat/Me.h>

void foxochat::events::Pong::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    Pong_t e;
    e.me = me;
    e.rawEvent = raw;
    if (callback != nullptr) {
        callback(e);
    }
}