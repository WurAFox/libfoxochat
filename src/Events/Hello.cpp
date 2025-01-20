#include <foxogram/Events.h>
#include <foxogram/Me.h>
#include <foxogram/Logger.h>

void foxogram::events::Hello::handle(foxogram::Me *me, nlohmann::json j, const std::string raw) {
    me->gateway.heartbeatInterval = j.at("d").at("heartbeat_interval").get<int>();
    me->gateway.running = true;
    me->gateway.pingThread = std::thread([me](){me->gateway.ping(me->gateway.heartbeatInterval);});
}