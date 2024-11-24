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
