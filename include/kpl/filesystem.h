#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <optional>

#include <kpl/core.h>
namespace kpl {
namespace filesystem
{
// I can use a template up here I guess. Hmm..

    bool isValidFilePath(kpl::String full_path);
    kpl::String removeExtension(const kpl::String& file_name);
    kpl::String& removeExtensionMut(kpl::String& file_name);
    QDir makeChildDir(const kpl::Directory& parent, const kpl::String& child_directory_name);
    bool fileExists(const kpl::Directory& directory, const kpl::String& file_name);

    class DirectoryPath
    {
    public:
        DirectoryPath(kpl::Directory dir);
        static std::optional<DirectoryPath> make(kpl::String path);

        kpl::String rootName() const;
        kpl::String leafName() const;
        kpl::String absolutePath() const;

    private:
        DirectoryPath(kpl::String full_path);

        kpl::String m_absolute_path;
    };

    class FileIdentifier
    {
    public:
        static std::optional<FileIdentifier> make(const kpl::String& absolute_path);
        static std::optional<FileIdentifier> make(DirectoryPath dir_path, kpl::String file_name);

        kpl::String absolutePath();
        kpl::String directoryAbsolutePath();
        kpl::String rootName();
        kpl::String fullName();
        kpl::String extensionName();

    private:
        FileIdentifier(DirectoryPath dir_path, kpl::String file_name);

        DirectoryPath m_file_directory;
        kpl::String m_file_name;
    };
}
}

#endif // FILESYSTEM_H
