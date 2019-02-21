#ifndef KFILE_H
#define KFILE_H

#include <string>
#include <sstream>
#include <fstream>

namespace kpl
{
    bool fileExists(const std::string& filePath);
    std::string loadTextFile(std::string filePath);
}
#endif // KFILE_H
