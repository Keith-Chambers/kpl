#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <optional>
#include <functional>

#include <kpl/core.h>
#include <kpl/string.h>

namespace kpl {
namespace filesystem
{

    class RelativePath;


    /*  Format
     *
     *  <root>/path/to/something
     *
     */
    class AbsolutePath
    {
    public:
        static std::optional<AbsolutePath> make(const kpl::String& path);

        std::optional<AbsolutePath> make(const RelativePath& relative_path) const;

        bool folderExists(const RelativePath& path) const;
        bool fileExists(const RelativePath& path) const;
        bool becomeChildPath(const RelativePath& path);

        bool isFolder() const;
        bool isFile() const;
        bool isRoot() const;

        kpl::String rootName() const;
        AbsolutePath parentPath() const;
        kpl::String leafName() const;
        kpl::String fullPath() const;

    private:
        AbsolutePath(const kpl::String& path);

        kpl::String m_path;
    };

    /*  Format
     *
     *  path/to/something
     *
     */
    class RelativePath
    {
    public:
        static std::optional<RelativePath> make(const kpl::String& path);
        std::optional<AbsolutePath> makeAbsolute(const AbsolutePath& root_path) const;

        kpl::String path() const;
    private:
        RelativePath(kpl::String path);

        kpl::String m_path;
    };


    class DirectoryPath
    {
    public:
        DirectoryPath(kpl::Directory dir);
        static std::optional<DirectoryPath> make(kpl::String path);
        static std::optional<kpl::filesystem::DirectoryPath> make(kpl::filesystem::DirectoryPath dir, const RelativePath& relative_path);

        kpl::String rootName() const;
        kpl::String leafName() const;
        kpl::String absolutePath() const;

        bool becomeSubDirectory(kpl::String directory_name);

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

    bool subdirectoryExists(kpl::filesystem::DirectoryPath dir, kpl::String sub_dir);

    bool writeFile(const DirectoryPath& dir_path, const kpl::String& file_name, const ContinguousMemory& memory);

    bool isValidFilePath(kpl::String full_path);
    kpl::String removeExtension(const kpl::String& file_name);
    kpl::String& removeExtensionMut(kpl::String& file_name);
    kpl::Directory makeChildDir(const kpl::Directory& parent, const kpl::String& child_directory_name);
    bool fileExists(const kpl::Directory& directory, const kpl::String& file_name);

    u32 countFilesInDirectoryRecursive(  kpl::Directory directory,
                                kpl::List<kpl::String> name_filters = {});

    kpl::List<kpl::filesystem::FileIdentifier> findFilesInDirectoryRecursive(
                                                                    kpl::Directory directory,
                                                                    kpl::List<kpl::String> name_filters = {});

    template <typename T>
    kpl::List<T> bestOf(kpl::List<T> items, u16 max, std::function<bool(T,T)> cmp);
}
}

#endif // FILESYSTEM_H
