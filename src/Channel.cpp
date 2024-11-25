#include <foxogram/Channel.h>

foxogram::Channel::Channel(long long int id, std::string name, int type, long long int ownerId) : id(id),
                                                                                                         name(name),
                                                                                                         type(type),
                                                                                                         ownerId(ownerId) {
    this->id = id;
    this->name = name;
    this->type = type;
    this->ownerId = ownerId;
}

void foxogram::Channel::deleteChannel(long long int id) {

}

void foxogram::Channel::edit(long long int id) {

}

void foxogram::Channel::leave(long long int id) {

}
