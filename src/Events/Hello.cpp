#include <foxochat/Events.h>
#include <foxochat/Me.h>
#include <foxochat/Logger.h>
#include <foxochat/Utils.h>

void foxochat::events::Hello::handle(foxochat::Me *me, nlohmann::json eventJ, const std::string raw) {
    nlohmann::json j = eventJ.at("d");
    me->gateway.heartbeatInterval = Utils::value<int>(j, "heartbeat_interval", 0);
    me->gateway.running = true;
    me->gateway.pingThread = std::thread([me](){me->gateway.ping(me->gateway.heartbeatInterval);});
    Hello_t e;
    e.me = me;
    e.rawEvent = raw;
    e.interval = me->gateway.heartbeatInterval;
    if (callback != nullptr) {
        callback(e);
    }
}