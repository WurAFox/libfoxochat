#include <foxogram/Message.h>
#include <foxogram/Channel.h>
#include <utility>

foxogram::Message::Message(long long int id, foxogram::Channel *channel, long long int authorId, long long int timestamp,
                           std::list<std::string> attachments) {
    this->id = id;
    this->channel = channel;
    this->authorId = authorId;
    this->timestamp = timestamp;
    this->attachments = std::move(attachments);
}
