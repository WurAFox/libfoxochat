#pragma once
#include <foxogram/export.h>
#include <foxogram/BaseEntity.h>
#include <foxogram/Proxy.h>
#include <map>
#include <string>
#include <iostream>


namespace foxogram {

    template<class T>
    class Cache {
    private:
        static_assert(std::is_base_of_v<BaseEntity, T>, "template must be base of BaseEntity");
        std::map<long long, std::shared_ptr<T>> cacheMap;
    public:
        std::shared_ptr<T> get(long long id) {
            auto it = cacheMap.find(id);
            return (it != cacheMap.end()) ? it->second : nullptr;
        }
        std::shared_ptr<T> store(std::shared_ptr<T> obj) {
            auto it = cacheMap.find(obj->getId());
            if (it == cacheMap.end()) {
                return cacheMap.insert({obj->getId(), obj}).first->second;
            } else {
                if (it->second != obj) {
                    it->second->~T();
                    new (it->second.get()) T(*obj);
                }
                return it->second;
            }
        }
        void Delete(long long id) {
            cacheMap.erase(id);
        }
        const std::map<long long, std::shared_ptr<T>>& getMap() const {
            return cacheMap;
        }
    };

}
