#ifndef GAMEHASHMAP_H
#define GAMEHASHMAP_H

#include <unordered_map>
#include <utility>

/*
 * Wrapper class surrounding a std::unordered_map.
 * This class is used in the Game Manager to hold all the Marines, Zombies, etc.
 * It has a single member, the internal map.
 * The majority of the methods simply call the unordered_map methods.
 * As such, they have been left uncommented as their return types, parameters, and specifications
 * can be obtained directly from the C++ standard documentation, and it's pointless for me to repeat it.
 * ~John Agapeyev March 21 2017
 */
template<typename T, typename U>
class GameHashMap {
public:
    GameHashMap() = default;
    ~GameHashMap() = default;

    typedef std::unordered_map<T, U> HashMap;

    /*
     * Returns a pair containing a value reference and a boolean.
     * The value reference is the element if it was found.
     * The boolean is whether or not the element currently exists in the map.
     * Please note that if the element is not found, the element returned is the result
     * of dereferencing the end iterator, resulting in undefined behaviour.
     * To prevent errors, always check the boolean in the pair before accessing the returned element
     * ~John Agapeyev March 21 2017
     */
    std::pair<U&, bool> operator[](T key) {
        const auto& it = internalMap.find(key);
        return {it->second, (it != internalMap.end())};
    }

    auto find(const T& key) const {
        return internalMap.find(key);
    }

    /*
     * Inserts a key-value pair into the map.
     * The traditional behaviour of an unordered_map is to only insert if there is no collision.
     * Due to the fact that our id's are generated in an ever increasing counter, we should never
     * realistically encounter a collision.
     * In the event a collision is generated though, this method overrides that behaviour and assigns
     * the requested insertion value to the element with the colliding key.
     * This guarantees that this method will always insert an element, barring some unknown exception.
     * ~John Agapeyev March 21 2017
     */
    auto insert(const typename HashMap::value_type& elem) {
        auto p = internalMap.insert(elem);
        if (!p.second) {
            p.first->second = elem.second;
        }
        return p.first;
    }

    /*
     * Merely an overload of the above insert method to match the unordered_map prototypes.
     * ~John Agapeyev March 21 2017
     */
    auto insert(typename HashMap::value_type&& elem) {
        auto p = internalMap.insert(elem);
        if (!p.second) {
            p.first->second = elem.second;
        }
        return p.first;
    }

    /*
     * Due to the fact that emplace can not insert an element in the event of
     * a collision, this method calls the custom insert method defined above and constructs an element
     * in-place using the parameter pack expansion.
     * This does not affect the construction attributes as passing an rvalue reference into insert
     * calls an overloaded emplace call, resulting in identical behaviour as just calling emplace with
     * the parameter pack.
     * ~John Agapeyev March 21 2017
     */
    template<typename... Args>
    auto emplace(Args&&... args) {
        return insert(typename HashMap::value_type(std::forward<Args>(args)...));
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
