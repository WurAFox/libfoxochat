#include <foxogram/Attachment.h>

#include <utility>
#include <foxogram/Utils.h>

namespace foxogram {
    Attachment Attachment::fromJson(const nlohmann::json& j) {
        return Attachment{Utils::value<long long>(j, "id", 0), Utils::value<std::string>(j, "uuid", ""),
            Utils::value<std::string>(j, "filename", ""), Utils::value<std::string>(j, "contentType", ""),
        Utils::value<long long>(j, "flags", 0)};
    }

    Attachment::Attachment(long long id, std::string uuid, std::string filename, std::string contentType,
        long long flags) : id(id), uuid(std::move(uuid)), filename(std::move(filename)), contentType(std::move(contentType)), flags(flags) {
    }

    long long Attachment::getId() const {
        return id;
    }

    std::string Attachment::getUuid() const {
        return uuid;
    }

    std::string Attachment::getFilename() const {
        return filename;
    }

    std::string Attachment::getContentType() const {
        return contentType;
    }

    AttachmentFlag Attachment::getFlags() const {
        return flags;
    }

    bool Attachment::hasFlag(AttachmentFlags flag) const {
        return flags.hasFlag(flag);
    }
} // foxogram