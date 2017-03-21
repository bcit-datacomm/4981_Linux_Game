#ifndef GAMEHASHMAP_H
#define GAMEHASHMAP_H

#include <unordered_map>
#include <utility>

template<typename T, typename U>
class GameHashMap {
public:
    GameHashMap() = default;
    ~GameHashMap() = default;

    typedef std::unordered_map<T, U> HashMap;

    std::pair<U&, bool> operator[](T key) {
        const auto& it = internalMap.find(key);
        return {it->second, (it != internalMap.end())};
    }

    auto find(const T& key) const {
        return internalMap.find(key);
    }

    auto insert(const typename HashMap::value_type& elem) {
        auto p = internalMap.insert(elem);
        if (!p.second) {
            p.first->second = elem.second;
        }
        return p.first;
    }
    
    auto insert(typename HashMap::value_type&& elem) {
        auto p = internalMap.insert(elem);
        if (!p.second) {
            p.first->second = elem.second;
        }
        return p.first;
    }

    template<typename... Args>
    auto emplace(Args&&... args) {
        return internalMap.emplace(std::forward(args)...);
    }

    void clear() {
        internalMap.clear();
    }

    void swap(GameHashMap& other) {
        std::swap(internalMap, other.internalMap);
    }

    auto erase(typename HashMap::const_iterator pos) {
        return internalMap.erase(pos);
    }

    auto erase(typename HashMap::const_iterator first, typename HashMap::const_iterator second) {
        return internalMap.erase(first, second);
    }
    
    auto erase(const T& key) {
        return internalMap.erase(key);
    }

    bool empty() const {
        return internalMap.empty();
    }

    auto size() const {
        return internalMap.size();
    }

    auto count(const T& key) const {
        return internalMap.count(key);
    }

    auto begin() {
        return internalMap.begin();
    }

    auto begin() const {
        return internalMap.begin();
    }

    auto cbegin() const {
        return internalMap.cbegin();
    }

    auto end() {
        return internalMap.end();
    }

    auto end() const {
        return internalMap.end();
    }

    auto cend() const {
        return internalMap.cend();
    }

    bool operator==(const GameHashMap& other) const {
        return internalMap == other.internalMap;
    }

    bool operator!=(const GameHashMap& other) const {
        return !(internalMap == other.internalMap);
    }

private:
    HashMap internalMap;
};

#endif

