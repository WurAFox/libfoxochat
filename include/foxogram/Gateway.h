#pragma once
#include <foxogram/export.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <nlohmann/json.hpp>

#ifndef FOXOGRAM_BASE_WEBSOCKET_URL
#ifdef IXWEBSOCKET_USE_TLS
#define FOXOGRAM_BASE_WEBSOCKET_URL "wss://api." FOXOGRAM_URL
#else
#define FOXOGRAM_BASE_WEBSOCKET_URL "ws://api." FOXOGRAM_URL
#endif
#endif

namespace foxogram {
    namespace events {
        class Hello;
        class Pong;
    }
    class LIBFOXOGRAM_EXPORT Gateway {
    protected:
        std::thread pingThread;
        friend events::Hello;
        friend events::Pong;
        class Me* me;
        int heartbeatInterval;
        static inline std::string wsUrl = FOXOGRAM_BASE_WEBSOCKET_URL;
        ix::WebSocket ws;
        std::mutex mtx;
        std::condition_variable cv;
        std::atomic<bool> running = true;
    public:
        explicit Gateway(Me* me, int heartbeatInterval = 10);
        void send(const nlohmann::json& data);
        void close();
        void ping(int interval);
        ~Gateway();
    };

}