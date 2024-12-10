#pragma once
#include <stdexcept>
#include <foxogram/HttpClient.h>

namespace foxogram {
    class LIBFOXOGRAM_EXPORT BaseEntity {
    protected:
        virtual void handleError(const nlohmann::json &response) const = 0;

    public:
        BaseEntity() = default;

        virtual ~BaseEntity() = default;
    };
}
