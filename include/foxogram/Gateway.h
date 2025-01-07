#pragma once
#include <foxogram/export.h>
#include <ixwebsocket/IXWebSocket.h>
#include <ixwebsocket/IXNetSystem.h>
#include <nlohmann/json.hpp>

#ifndef FOXOGRAM_BASE_WEBSOCKET_URL
#ifdef IXWEBSOCKET_USE_TLS
#define FOXOGRAM_BASE_WEBSOCKET_URL "wss://gateway." FOXOGRAM_URL
#else
#define FOXOGRAM_BASE_WEBSOCKET_URL "ws://gateway." FOXOGRAM_URL
#endif
#endif

namespace foxogram {

    class LIBFOXOGRAM_EXPORT Gateway {
    protected:
        class Me* me;
        int heartbreakInterval = 10;
        static inline std::string wsUrl = FOXOGRAM_BASE_WEBSOCKET_URL;
        ix::WebSocket ws;
    public:
        explicit Gateway(Me* me, int heartbreakInterval = 10);
        void send(nlohmann::json data);
        void close();
    };

}