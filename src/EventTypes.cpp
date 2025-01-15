#include <foxogram/EventTypes.h>

foxogram::Event_t::Event_t(Me* me, const std::string& raw) : me(me), rawEvent(raw)  {}
foxogram::Event_t::Event_t(Me* me, std::string&& raw) : me(me), rawEvent(raw)  {}
