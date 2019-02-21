#include "logger.h"

namespace kpl
{

    Logger * Logger::mInstance = nullptr;

    Logger * Logger::getInstance(size_t bufferSize)
    {
        if(! mInstance )
            mInstance = new Logger(bufferSize);

        return mInstance;
    }

    Logger::Logger(size_t bufferSize)
    {
        mLogData = new char[bufferSize];
        *mLogData = '\0';
        mLogDataBufferSize = bufferSize;
        mLogDataContentSize = 0;
    }

    bool Logger::log(const char * text, size_t size)
    {
        return fixedCStringAppendNL(text, size);
    }

    bool Logger::log(char * text, size_t size)
    {
        return fixedCStringAppendNL(text, size);
    }

    bool Logger::log(const char * text)
    {
        size_t size = strlen(text);
        return fixedCStringAppendNL(text, size);
    }

    bool Logger::log(char * text)
    {
        size_t size = strlen(text);
        return fixedCStringAppendNL(text, size);
    }

    bool Logger::logSameLine(const char * text)
    {
        size_t size = strlen(text);
        return fixedCStringAppend(text, size);
    }

    bool Logger::logSameLine(char * text)
    {
        size_t size = strlen(text);
        return fixedCStringAppend(text, size);
    }

    char * Logger::cString(size_t * stringSize)
    {
        if(stringSize)
            *stringSize = mLogDataContentSize;

        return mLogData;
    }

    size_t Logger::getContentSize()
    {
        return mLogDataContentSize;
    }

    size_t Logger::getBufferSize()
    {
        return mLogDataBufferSize;
    }

    void Logger::clear()
    {
        mLogDataContentSize = 0;
    }

    bool Logger::copyToCBuffer(char * buffer, size_t bufferSize)
    {
        if(mLogDataContentSize == 0)
            return false;

        long int spaceMissing = static_cast<long int>(mLogDataContentSize - bufferSize);

        // Clamp to a pos value
        spaceMissing = (spaceMissing < 0) ? 0 : spaceMissing;

        strncpy(buffer, mLogData + spaceMissing, bufferSize - 1);
        buffer[bufferSize - 1] = '\0';

        return true;
    }

    void Logger::shiftCStringLeft(char * string, size_t *stringSize, size_t shiftAmount)
    {
        if(shiftAmount == 0)
            return;

        // Start from the end,
        char * current = string + (*stringSize);
        char * shiftTo = current - shiftAmount;

        while(shiftTo != string)
        {
            *shiftTo = *current;
            shiftTo--;
            current--;
        }

        *(stringSize) -= shiftAmount;
    }

    bool Logger::fixedCStringAppend(const char * newLog, size_t size)
    {
        if(size > (mLogDataBufferSize - 1))
        {
            std::cout << "Can't fit \'" << newLog << "\' into log buffer" << std::endl;
            return false;
        }

        long int spaceMissing = static_cast<long int>((size + mLogDataContentSize) - mLogDataBufferSize);

        // Clamp to a pos value
        spaceMissing = (spaceMissing < 0) ? 0 : spaceMissing;

        if(spaceMissing)
            shiftCStringLeft(mLogData, &mLogDataContentSize, static_cast<size_t>(spaceMissing));

        for(size_t i = 0; i < size; i++)
            *(mLogData + mLogDataContentSize + i) = *(newLog + i);

        mLogDataContentSize += size;
        
        *(mLogData + mLogDataContentSize) = '\0';

        return true;
    }

    bool Logger::fixedCStringAppendNL(const char * newLog, size_t size)
    {
        bool prependNewLine = (mLogDataContentSize != 0);
        
        std::cout << "Append w/ NL: " << newLog << "; Size: " << size << std::endl;

        if(size > (mLogDataBufferSize - 1))
            return false;

        if(prependNewLine)
        {
            // Overwrites null terminated character
            *(mLogData + mLogDataContentSize) = '\n';
            mLogDataContentSize++;
        }

        long int spaceMissing = static_cast<long int>((size + mLogDataContentSize) - mLogDataBufferSize);

        // Clamp to a pos value
        spaceMissing = (spaceMissing < 0) ? 0 : spaceMissing;

        if(spaceMissing)
            shiftCStringLeft(mLogData, &mLogDataContentSize, static_cast<size_t>(spaceMissing));

        for(size_t i = 0; i < size; i++)
            *(mLogData + mLogDataContentSize + i) = *(newLog + i);

        mLogDataContentSize += size;
        
        *(mLogData + mLogDataContentSize) = '\0';

        return true;
    }

}
