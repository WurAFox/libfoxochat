#pragma once
#include <foxogram/export.h>
#include <foxogram/BaseEntity.h>
#include <map>
#include <string>

namespace foxogram {

    template<class T>
    class Cache {
    private:
        static_assert(std::is_base_of<BaseEntity, T>::value, "template must be base of BaseEntity");
        std::map<std::string, T*> cacheMap;
    public:
        Cache() = default;
        T* get(std::string name) {
            auto it = cacheMap.find(name);
            return (it != cacheMap.end()) ? it->second : nullptr;
        }
        T* get(long long id) {
            return get(std::to_string(id));
        }
        void store(T* obj) {
            auto it = cacheMap.find(obj->getIdOrName());
            if (it == cacheMap.end()) {
                cacheMap.insert({obj->getIdOrName(), obj});
            } else {
                if (it->second != obj) {
                    it->second = obj;
                }
            }
        }
    };

}
