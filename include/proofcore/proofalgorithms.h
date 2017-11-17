#ifndef PROOFALGORITHMS_H
#define PROOFALGORITHMS_H

#include <type_traits>

namespace Proof {
namespace algorithms {
template<typename Container, typename Predicate>
auto eraseIf(Container &container, const Predicate &predicate)
-> decltype(predicate(container.cbegin().key(), container.cbegin().value()),
            void())
{
    for (auto it = container.begin(); it != container.end();) {
        if (predicate(it.key(), it.value()))
            it = container.erase(it);
        else
            ++it;
    }
}

template<typename Container, typename Predicate>
auto eraseIf(Container &container, const Predicate &predicate)
-> decltype(predicate(*(container.cbegin())),
            void())
{
    container.erase(std::remove_if(container.begin(), container.end(), predicate), container.end());
}

template<typename Container>
void makeUnique(Container &container)
{
    container.erase(std::unique(container.begin(), container.end()), container.end());
}

template<typename Container, typename Predicate, typename Result = typename Container::value_type>
auto findIf(const Container &container, const Predicate &predicate, const Result &defaultValue = Result())
-> decltype(predicate(*(container.cbegin())),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it) {
        if (predicate(*it))
            return *it;
    }
    return defaultValue;
}

template<typename Container, typename Predicate, typename Result = QPair<typename Container::key_type, typename Container::mapped_type>>
auto findIf(const Container &container, const Predicate &predicate, const Result &defaultValue = Result())
-> decltype(predicate(container.cbegin().key(), container.cbegin().value()),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it) {
        if (predicate(it.key(), it.value()))
            return qMakePair(it.key(), it.value());
    }
    return defaultValue;
}

template<typename C, typename T>
auto addToContainer(C &container, const T &value) -> decltype(container.push_back(value), void())
{
    container.push_back(value);
}

template<typename C, typename T>
auto addToContainer(C &container, const T &value) -> decltype(container.insert(value), void())
{
    container.insert(value);
}

template<typename C, typename T>
auto addToContainer(C &container, const T &value) -> decltype(container.insert(value.key(), value.value()), void())
{
    container.insert(value.key(), value.value());
}

template<typename C, typename T>
auto addToContainer(C &container, const T &value) -> decltype(container.insert(value.first, value.second), void())
{
    container.insert(value.first, value.second);
}

// filter, map and reduce are not lazy, they copy values.
// It is fine for almost all our cases with DTOs and PODs, but still is O(n).
// O(n) though is good enough for our amounts of data.
// If ever lazy variant will be needed probably we will need to use boost ranges or something similar then.
template<typename Container, typename Predicate>
auto filter(const Container &container, const Predicate &predicate)
-> decltype(addToContainer(const_cast<Container&>(container), *(container.cbegin())),
            predicate(*(container.cbegin())),
            Container())
{
    Container result;
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it) {
        if (predicate(*it))
            addToContainer(result, *it);
    }
    return result;
}

template<typename Container, typename Predicate>
auto filter(const Container &container, const Predicate &predicate)
-> decltype(addToContainer(const_cast<Container&>(container), container.cbegin()),
            predicate(container.cbegin().key(), container.cbegin().value()),
            Container())
{
    Container result;
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it) {
        if (predicate(it.key(), it.value()))
            addToContainer(result, it);
    }
    return result;
}

template<typename Container, typename Predicate, class Result>
auto map(const Container &container, const Predicate &predicate, Result destination)
-> decltype(addToContainer(destination, predicate(*(container.cbegin()))),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it)
        addToContainer(destination, predicate(*it));
    return destination;
}

template<typename Container, typename Predicate, class Result>
auto map(const Container &container, const Predicate &predicate, Result destination)
-> decltype(addToContainer(destination, predicate(container.cbegin().key(), container.cbegin().value())),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it)
        addToContainer(destination, predicate(it.key(), it.value()));
    return destination;
}

template<typename Container, typename Predicate, typename Result>
auto reduce(const Container &container, const Predicate &predicate, Result acc)
-> decltype(acc = predicate(acc, *(container.cbegin())),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it)
        acc = predicate(acc, *it);
    return acc;
}

template<typename Container, typename Predicate, typename Result>
auto reduce(const Container &container, const Predicate &predicate, Result acc)
-> decltype(acc = predicate(acc, container.cbegin().key(), container.cbegin().value()),
            Result())
{
    auto it = container.cbegin();
    auto end = container.cend();
    for (; it != end; ++it)
        acc = predicate(acc, it.key(), it.value());
    return acc;
}

}
}

#endif // PROOFALGORITHMS_H