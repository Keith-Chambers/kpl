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

    std::string Util::lowerFirstChar(std::string str)
    {
        std::string result { str };
        result[0] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[0])));
        return result;
    }

    std::string Util::upperFirstChar(std::string str)
    {
        std::string result { str };
        result[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(result[0])));
        return result;
    }

    void Util::indentString(std::string& targetString, const std::string& indentationString, uint16_t indentLevels)
    {
        std::string::size_type currentPos = 0;
        std::string placeHolder = "\n";

        std::string indentation;
        while(indentLevels-- > 0)
            indentation += indentationString;

        while(currentPos != std::string::npos)
        {
            currentPos = targetString.find(placeHolder, currentPos);
            targetString.replace(currentPos, placeHolder.length(), "\n" + indentation);
            currentPos += indentation.length() + 1;
        }
    }
}


