#include <foxochat/Gateway.h>
#include <foxochat/Me.h>
#include <foxochat/exceptions.h>
#include <foxochat/Logger.h>
#include <foxochat/Utils.h>

foxochat::Gateway::Gateway(foxochat::Me *me, int heartbeatInterval) : me(me), heartbeatInterval(heartbeatInterval) {
    ix::initNetSystem();
    ws.setUrl(wsUrl);
    ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg){
        if (msg->type == ix::WebSocketMessageType::Message) {
            foxochat::Logger::logDebug("Get message from gateway: " + msg->str);
            auto j = nlohmann::json::parse(msg->str);
            switch (Utils::value<int>(j, "op", 0)) {
                case 0: {
                    auto it = this->me->eventMap.find(Utils::value<std::string>(j, "t", ""));
                    if (it != this->me->eventMap.end()) {
                        it->second->handle(this->me, j, to_string(j));
                    } else {
                        Logger::logWarning("Unknown dispatch event: " + Utils::value<std::string>(j, "t", ""));
                    }
                    break;
                }
                case 2: {
                    auto it = this->me->eventMap.find("HELLO");
                    if (it != this->me->eventMap.end()) {
                        it->second->handle(this->me, j, to_string(j));
                    } else {
                        Logger::logWarning("Could not find Hello event handler ");
                    }
                    break;
                }
                case 4: {
                    auto it = this->me->eventMap.find("PONG");
                    if (it != this->me->eventMap.end()) {
                        it->second->handle(this->me, j, to_string(j));
                    } else {
                        Logger::logWarning("Could not find pong event handler ");
                    }
                    break;
                }
            }

        } else if (msg->type == ix::WebSocketMessageType::Error) {
            foxochat::Logger::logError(msg->errorInfo.reason);
            throw foxochat::WebSocketException("Error while connecting to websocket: " + msg->errorInfo.reason);
        } else if (msg->type == ix::WebSocketMessageType::Open) {
            send(nlohmann::json::parse(R"({"op": 1, "d": {
                    "token": ")" + *this->me->token + R"("
                }
            })"));
        } else if (msg->type == ix::WebSocketMessageType::Close) {
            Logger::logWarning("Closing websocket with reason: " + msg->closeInfo.reason);
        }
    });
    ws.start();
}

void foxochat::Gateway::send(const nlohmann::json& data) {
    if (ws.getReadyState() == ix::ReadyState::Open) {
        ws.send(to_string(data));
    }
    else {
        throw foxochat::WebSocketException(std::string("Websocket not opened"));
    }
}

void foxochat::Gateway::close() {
    if (this->ws.getReadyState() == ix::ReadyState::Open) {
        this->ws.stop();
    }
}

void foxochat::Gateway::ping(int interval) {
    std::unique_lock<std::mutex> lock(mtx);

    while (running) {
        send(nlohmann::json{{"op", 3}});
        if (cv.wait_for(lock, std::chrono::milliseconds(interval), [this](){ return !running; })) {
            break;
        }
    }
}

foxochat::Gateway::~Gateway() {
    running = false;
        cv.notify_one();
    pingThread.join();
}