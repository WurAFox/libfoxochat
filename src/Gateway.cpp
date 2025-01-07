#include <foxogram/Gateway.h>
#include <foxogram/Me.h>
#include <foxogram/exceptions.h>
#include <foxogram/Logger.h>

foxogram::Gateway::Gateway(foxogram::Me *me, int heartbreakInterval) : me(me), heartbreakInterval(heartbreakInterval) {
    ix::initNetSystem();
    ws.setUrl(wsUrl);
    ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg){
        if (msg->type == ix::WebSocketMessageType::Message) {
            foxogram::Logger::logDebug("Get message from gateway: " + msg->str);
            auto j = nlohmann::json::parse(msg->str);
        }
        else if (msg->type == ix::WebSocketMessageType::Error) {
            foxogram::Logger::logDebug(msg->errorInfo.reason);
            throw foxogram::WebSocketException("Error while connecting to websocket: " + msg->errorInfo.reason);
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            send(nlohmann::json::parse(R"({"op": 0, "d": {
                    "token": ")" + *this->me->token + R"("
                }
            })"));
        }
    });
    ws.start();
}

void foxogram::Gateway::send(nlohmann::json data) {
    if (ws.getReadyState() == ix::ReadyState::Open) {
        ws.send(to_string(data));
    }
    else {
        throw foxogram::WebSocketException(std::string("Websocket not opened"));
    }
}

void foxogram::Gateway::close() {
    if (this->ws.getReadyState() == ix::ReadyState::Open) {
        this->ws.close();
        this->ws.stop();
    }
}