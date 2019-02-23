#ifndef UTIL_H
#define UTIL_H

#define ANAGAMI_DEBUG

#ifdef ANAGAMI_DEBUG

#include <fmt/format.h>
#include <assert.h>
#endif

#include <string>
#include <cstring>

namespace kpl {

    class Util
    {
    public:
        static const std::string constEmptyString;

        /* Warning: Not safe. Check memory block ranges before invoking */
        static bool dataRightShift(uint8_t * memoryBlockStart, uint16_t numElements, size_t elementSize, uint16_t shiftSize = 1);

        Util();
    };
}

#endif // UTIL_H