#pragma once

#include <nlohmann/json.hpp>
#include <foxochat/export.h>
#include <foxochat/Enums.h>

namespace foxochat {

    struct LIBFOXOCHAT_EXPORT Attachment {
    private:
        long long id;
        std::string uuid;
        std::string filename;
        std::string contentType;
        FlagsBase<AttachmentFlags> flags;
    public:
        [[nodiscard]] static Attachment fromJson(const nlohmann::json& j);
        Attachment(long long id, std::string uuid, std::string filename, std::string contentType, long long flags);
        [[nodiscard]] long long getId() const;
        [[nodiscard]] std::string getUuid() const;
        [[nodiscard]] std::string getFilename() const;
        [[nodiscard]] std::string getContentType() const;
        [[nodiscard]] FlagsBase<AttachmentFlags> getFlags() const;
        [[nodiscard]] bool hasFlag(AttachmentFlags flag) const;
    };
} // foxochat
