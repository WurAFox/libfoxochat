#pragma once
#include <stdexcept>
#include <foxogram/HttpClient.h>

namespace foxogram {
    class LIBFOXOGRAM_EXPORT BaseEntity {
    protected:
        long long id;

        virtual void handleError(const nlohmann::json &response) const = 0;

    public:
        explicit BaseEntity(const long long id) : id(id) {
        }

        virtual ~BaseEntity() = default;

        [[nodiscard]] long long getId() const { return id; }

        virtual void validate() const {
            if (id <= 0) {
                throw std::invalid_argument("Invalid entity ID");
            }
        }
    };
}
