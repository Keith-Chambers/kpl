#include "kpl/kfile.h"

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


    bool createFile(const std::filesystem::path path, bool force)
    {
        if(fileExists(path) && ! force)
        {
            std::cout << path << " already exists. Exiting" << std::endl;
            return false;
        }

        std::ofstream newFile (path);
        newFile.close();

        return true;
    }

    bool truncateFile(const std::filesystem::path path)
    {
        if(! fileExists(path))
        {
            std::cout << path << " doesn't exist. Exiting truncateFile function" << std::endl;
            return false;
        }

        std::ofstream truncateStream;
        truncateStream.open(path, std::ofstream::trunc);
        truncateStream.close();

        return true;
    }
}
