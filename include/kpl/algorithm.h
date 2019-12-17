#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <kpl/core.h>

namespace kpl
{
    template <typename T>
    u32 countElementsFromEnd(ContinguousView<T> data, const T& find);

    template <typename T>
    u32 countElementsFromBegin(ContinguousView<T> data, const T& find);

    template <typename T>
    ContinguousView<T> tail(ContinguousView<T> data, u32 length);

    template <typename T>
    ContinguousView<T> head(ContinguousView<T> data, u32 length);


    // Function Alias'
    template<typename T>
    constexpr auto elementsFromEnd = tail<T>;

    template<typename T>
    constexpr auto elementsFromBegin = head<T>;
}

#endif // ALGORITHM_H
