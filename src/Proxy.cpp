#include <foxogram/Proxy.h>
#include <foxogram/Channel.h>
#include <foxogram/Member.h>
#include <foxogram/User.h>
#include <foxogram/Message.h>

namespace foxogram {
    MemberPtr Proxy<struct Channel>::getMember(long long int id) {
        return ptr->getMember(id);
    }

    MemberPtr Proxy<struct Channel>::fetchMember(long long int id) {
        return ptr->fetchMember(id);
    }

    std::list<MemberPtr> Proxy<struct Channel>::fetchMembers() {
        return ptr->fetchMembers();
    }

    std::list<MemberPtr> Proxy<struct Channel>::getMembers() const {
        return ptr->getMembers();
    }

    std::string Proxy<struct Channel>::getOwnerName() const {
        return ptr->getOwnerName();
    }

    int Proxy<struct Channel>::getType() const {
        return ptr->getType();
    }

    std::string Proxy<struct Channel>::getName() const {
        return ptr->getName();
    }

    MessagePtr Proxy<struct Channel>::createMessage(std::string content, const std::list<std::string> &attachments) {
        return foxogram::MessagePtr();
    }

    MessagePtr Proxy<struct Channel>::fetchMessage(long long int id) {
        return ptr->fetchMessage(id);
    }

    std::list<MessagePtr> Proxy<struct Channel>::fetchMessages(long long before, int after) {
        return ptr->fetchMessages(before, after);
    }

    std::list<MessagePtr> Proxy<struct Channel>::getMessages() const {
        return ptr->getMessages();
    }

    bool Proxy<struct Channel>::deleteChannel() {
        return ptr->deleteChannel();
    }

    void Proxy<struct Channel>::edit(const std::string &displayName, const std::string &name, const std::string &icon) {
        ptr->edit(displayName, name, icon);
    }

    bool Proxy<struct Channel>::leave() {
        return ptr->leave();
    }

    const std::string &Proxy<struct Channel>::getIcon() const {
        return ptr->getIcon();
    }

    const std::string &Proxy<struct Channel>::getDisplayName() const {
        return ptr->getDisplayName();
    }

    long long int Proxy<struct Channel>::getCreatedAt() const {
        return ptr->getCreatedAt();
    }

    const std::string& Proxy<User>::getDisplayName() const {
        return ptr->getDisplayName();
    }

    int Proxy<User>::getType() const {
        return ptr->getType();
    }

    long long Proxy<User>::getCreatedAt() const {
        return ptr->getCreatedAt();
    }

    void Proxy<struct User>::addFlag(UserFlags flag) {
        ptr->addFlag(flag);
    }

    void Proxy<struct User>::removeFlag(UserFlags flag) {
        ptr->removeFlag(flag);
    }

    bool Proxy<struct User>::hasFlag(UserFlags flag) const {
        return ptr->hasFlag(flag);
    }

    const std::string &Proxy<struct User>::getUsername() const {
        return ptr->getUsername();
    }

    const std::string &Proxy<struct User>::getAvatar() const {
        return ptr->getAvatar();
    }

    FlagsBase<UserFlags> Proxy<struct User>::getFlags() const {
        return ptr->getFlags();
    }

    void Proxy<struct Message>::deleteMessage() const {
        ptr->deleteMessage();
    }

    void Proxy<struct Message>::edit() const {
        ptr->edit();
    }

    long long int Proxy<struct Message>::getChannelId() const {
        return ptr->getChannelId();
    }

    long long int Proxy<struct Message>::getCreatedAt() const {
        return ptr->getCreatedAt();
    }


    long long int Proxy<Member>::getChannelId() const {
        return ptr->getChannelId();
    }

    const FlagsBase<MemberPermissions> &Proxy<struct Member>::getPermissions() const {
        return ptr->getPermissions();
    }

    void Proxy<struct Member>::addPermissions(MemberPermissions permission) {
        ptr->addPermissions(permission);
    }

    void Proxy<struct Member>::removePermissions(MemberPermissions permission) {
        ptr->removePermissions(permission);
    }

    bool Proxy<struct Member>::hasPermissions(MemberPermissions permission) const {
        return ptr->hasPermissions(permission);
    }

    long long int Proxy<struct Message>::getAuthorId() const {
        return ptr->getAuthorId();
    }

    const std::list<std::string> &Proxy<struct Message>::getAttachments() const {
        return ptr->getAttachments();
    }

    const std::string &Proxy<struct Message>::getContent() const {
        return ptr->getContent();
    }

    std::shared_ptr<Proxy<Member>> Proxy<struct Message>::getAuthor() const {
        return ptr->getAuthor();
    }

}