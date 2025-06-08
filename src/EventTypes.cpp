#include <foxochat/EventTypes.h>

foxochat::Event_t::Event_t(Me* me, const std::string& raw) : me(me), rawEvent(raw)  {}
foxochat::Event_t::Event_t(Me* me, std::string&& raw) : me(me), rawEvent(raw)  {}
