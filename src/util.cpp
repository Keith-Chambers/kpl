#include "kpl/util.h"

namespace kpl {

    const std::string Util::constEmptyString = "";

    Util::Util()
    {

    }

    bool Util::dataRightShift(uint8_t * memoryBlockStart, uint16_t numElements, size_t elementSize, uint16_t shiftSize)
    {
        if(shiftSize == 0)
        {
            #ifdef ANAGAMI_DEBUG

            fmt::print("Warning: shiftSize of {} passed to ChainedArray::dataRightShift\n");
            assert(false);

            #endif

            return true;
        }

        std::memmove(memoryBlockStart + shiftSize, memoryBlockStart,  numElements * elementSize);

        return true;
    }
}


