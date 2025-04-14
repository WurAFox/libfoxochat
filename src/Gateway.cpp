#include <foxogram/Gateway.h>
#include <foxogram/Me.h>
#include <foxogram/exceptions.h>
#include <foxogram/Logger.h>

foxogram::Gateway::Gateway(foxogram::Me *me, int heartbeatInterval) : me(me), heartbeatInterval(heartbeatInterval) {
    ix::initNetSystem();
    ws.setUrl(wsUrl);
    ws.setOnMessageCallback([this](const ix::WebSocketMessagePtr& msg){
        if (msg->type == ix::WebSocketMessageType::Message) {
            foxogram::Logger::logDebug("Get message from gateway: " + msg->str);
            auto j = nlohmann::json::parse(msg->str);
            switch (j.value<int>("op", 0)) {
                case 0: {
                    auto it = this->me->eventMap.find(j.value<std::string>("t", ""));
                    if (it != this->me->eventMap.end()) {
                        it->second->handle(this->me, j, to_string(j));
                    } else {
                        Logger::logWarning("Unknown dispatch event: " + j.value<std::string>("t", ""));
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
            foxogram::Logger::logError(msg->errorInfo.reason);
            throw foxogram::WebSocketException("Error while connecting to websocket: " + msg->errorInfo.reason);
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

void foxogram::Gateway::send(const nlohmann::json& data) {
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

void foxogram::Gateway::ping(int interval) {
    std::unique_lock<std::mutex> lock(mtx);

    while (running) {
        send(nlohmann::json{{"op", 3}});
        if (cv.wait_for(lock, std::chrono::milliseconds(interval), [this](){ return !running; })) {
            break;
        }
    }
}

foxogram::Gateway::~Gateway() {
    running = false;
        cv.notify_one();
    pingThread.join();
}