#include "kfile.h"

namespace kpl
{
    bool fileExists(const std::string& filePath)
    {
        if (FILE *file = fopen(filePath.c_str(), "r"))
        {
            fclose(file);
            return true;
        }

        return false;
    }

    std::string loadTextFile(std::string filePath)
    {
        std::ifstream input(filePath);
        std::stringstream buffer;
        buffer << input.rdbuf();

        // Make sure to copy construct, may not be needed
        return std::string(buffer.str());
    }
}
