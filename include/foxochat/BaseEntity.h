#pragma once
#include <iostream>
#include <stdexcept>
#include <foxochat/HttpClient.h>

namespace foxochat {
    class LIBFOXOCHAT_EXPORT BaseEntity {
            protected:
            long long id;

            virtual void handleError(const nlohmann::json &response) const = 0;

            public:
            explicit BaseEntity(const long long id) : id(id) {
            }

            virtual ~BaseEntity() = default;

            [[nodiscard]] long long getId() const { return id; }
    };
}
