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

    std::string Util::encloseQuotes(const std::string str)
    {
        return "'" + str + "'";
    }

    std::string Util::makeStringUpper(std::string str)
    {
        for(size_t i = 0; i < str.size(); i++)
            str[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(str[i])));

        return str;
    }

    void Util::createFile(const std::string& path, const std::string& contents)
    {
        std::ofstream outfile { path };
        outfile << contents << std::endl;
        outfile.close();
    }

    std::string Util::loadTextFile(const std::string& filePath)
    {
        std::ifstream fileToLoad(filePath);
        std::string result( (std::istreambuf_iterator<char>(fileToLoad) ),
                            (std::istreambuf_iterator<char>()    ) );
        return result;
    }

    void Util::prependFile(const std::string& filePath, std::string content)
    {
        // TODO: Check file size and define a max limit
        content += Util::loadTextFile(filePath);
        createFile(filePath, content);
    }

    void Util::copyFileUnix(const std::string& srcPath, const std::string& destPath, const std::string& fileName)
    {
        system( std::string("cp " + srcPath + "/" + fileName + " " + destPath + "/" + fileName).c_str() );
    }

    bool Util::appendContentToFile(const std::string& filePath, const std::string& contents)
    {
        std::ofstream outfile;

        outfile.open(filePath, std::ios::app);
        outfile << "\n" + contents;

        outfile.close();

        return true;
    }

    std::string Util::makeStringLower(std::string str)
    {
        for(size_t i = 0; i < str.size(); i++)
            str[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(str[i])));

        return str;
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

        while(true)
        {
            currentPos = targetString.find(placeHolder, currentPos);

            if(currentPos == std::string::npos)
                break;

            targetString.replace(currentPos, placeHolder.length(), "\n" + indentation);
            currentPos += indentation.length();
        }
    }
}


