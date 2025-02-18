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
        static_assert(std::is_base_of<BaseEntity, T>::value, "template must be base of BaseEntity");
        std::map<long long, std::shared_ptr<Proxy<T>>> cacheMap;
    public:
        Cache() = default;
        std::shared_ptr<Proxy<T>> get(long long id) {
            auto it = cacheMap.find(id);
            return (it != cacheMap.end()) ? it->second : nullptr;

        }
        std::shared_ptr<Proxy<T>> store(std::shared_ptr<T> obj) {
            std::shared_ptr<Proxy<T>> proxyObj;
            auto it = cacheMap.find(obj->getId());
            if (it == cacheMap.end()) {
                auto proxy = std::make_shared<Proxy<T>>();
                proxy->ptr = obj;
                return cacheMap.insert({obj->getId(), proxy}).first->second;
            } else {
                if (it->second->ptr != obj) {
                    it->second->ptr = obj;
                }
                return it->second;
            }
        }
        void Delete(long long id) {
            cacheMap.erase(id);
        }
        const std::map<long long, std::shared_ptr<Proxy<T>>>& getMap() const {
            return cacheMap;
        }
    };

}
