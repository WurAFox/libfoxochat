#pragma once
#include <foxogram/export.h>
#include <foxogram/BaseEntity.h>
#include <map>
#include <string>
#include <iostream>

namespace foxogram {

    template<class T>
    class Cache {
    private:
        static_assert(std::is_base_of<BaseEntity, T>::value, "template must be base of BaseEntity");
        std::map<long long, std::shared_ptr<T>> cacheMap;
    public:
        Cache() = default;
        std::shared_ptr<T> get(long long id) {
            auto it = cacheMap.find(id);
            return (it != cacheMap.end()) ? it->second : nullptr;

        }
        void store(std::shared_ptr<T> obj) {
            auto it = cacheMap.find(obj->getId());
            if (it == cacheMap.end()) {
                cacheMap.insert({obj->getId(), obj});
            } else {
                if (it->second != obj) {
                    it->second = obj;
                }
            }
        }
        const std::map<long long, std::shared_ptr<T>>& getMap() const {
            return cacheMap;
        }
    };

}
