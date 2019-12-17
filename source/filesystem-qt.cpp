#include <kpl/filesystem.h>

// Qt Implementation

namespace kpl {
namespace filesystem {


    kpl::String concatPaths(kpl::String absolute_path, kpl::String relative_path)
    {
        return absolute_path + "/" + relative_path;
    }

    std::optional<AbsolutePath> AbsolutePath::make(const kpl::String& path)
    {
        kpl::String formatted_path = kpl::trimEnd(path, '/');

        // TODO: String validation
        return std::optional<AbsolutePath>({formatted_path});
    }

    bool AbsolutePath::isFolder() const
    {
        return QDir(m_path).exists();
    }

    bool AbsolutePath::isRoot() const
    {
        return QDir(m_path).isRoot();
    }

    AbsolutePath AbsolutePath::parentPath() const
    {
        return AbsolutePath(kpl::forwardUntilLast(m_path, '/'));
    }

    bool AbsolutePath::isFile() const
    {
        return QFile(fullPath()).exists();
    }

    kpl::String AbsolutePath::rootName() const
    {
        return QDir(m_path).root().path();
    }

    kpl::String AbsolutePath::leafName() const
    {
        return QDir(m_path).dirName();
    }

    kpl::String AbsolutePath::fullPath() const
    {
        return m_path;
    }

    AbsolutePath::AbsolutePath(const kpl::String& path)
        : m_path {path}
    {}

    std::optional<AbsolutePath> AbsolutePath::make(const RelativePath& relative_path) const
    {
        return AbsolutePath::make( concatPaths(m_path, relative_path.path()) );
    }

    bool AbsolutePath::folderExists(const RelativePath& path) const
    {
        std::optional<AbsolutePath> child_path = make(path);

        if(child_path == std::nullopt) {
            return false;
        }

        return QDir(child_path.value().fullPath()).exists();
    }

    bool AbsolutePath::fileExists(const RelativePath& path) const
    {
        std::optional<AbsolutePath> child_path = make(path);

        if(child_path == std::nullopt) {
            return false;
        }

        return QFile(child_path.value().fullPath()).exists();
    }

    bool AbsolutePath::becomeChildPath(const RelativePath& path)
    {
        std::optional<AbsolutePath> child_path = make(path);

        if(child_path == std::nullopt) {
            return false;
        }

        m_path = child_path->fullPath();

        return true;
    }

    std::optional<RelativePath> RelativePath::make(const kpl::String& path)
    {
        // TODO: Path validation
        return std::optional<RelativePath>({path});
    }

    kpl::String RelativePath::path() const
    {
        return m_path;
    }

    std::optional<AbsolutePath> RelativePath::makeAbsolute(const AbsolutePath& root_path) const
    {
        return AbsolutePath::make( concatPaths(root_path.fullPath(), m_path) );
    }

    RelativePath::RelativePath(kpl::String path)
    {
        kpl::trimEndMut(path, '/');
        kpl::trimEndMut(path, ' ');
        kpl::trimEndMut(path, '\n');

        m_path = path;
    }


    bool isValidFilePath(QString full_path)
    {
        return QDir(full_path).exists();
    }

    std::optional<DirectoryPath> DirectoryPath::make(const QString& path)
    {
        if(!kpl::filesystem::isValidFilePath(path)) {
            return std::nullopt;
        }

        return std::optional<DirectoryPath>(DirectoryPath(QDir(path).absolutePath()));
    }

    DirectoryPath::DirectoryPath(const QString& full_path)
        : m_absolute_path{ full_path }
    {}

    DirectoryPath::DirectoryPath(QDir dir)
        : m_absolute_path{ dir.absolutePath() }
    {}

    QString DirectoryPath::rootName() const
    {
        return QDir(m_absolute_path).root().path();
    }

    QString DirectoryPath::leafName() const
    {
        return QDir(m_absolute_path).dirName();
    }

    QString DirectoryPath::absolutePath() const
    {
        return m_absolute_path;
    }

    std::optional<DirectoryPath> DirectoryPath::make(const AbsolutePath& path)
    {
        if(!path.isFolder()) {
            return std::nullopt;
        }

        return std::optional<DirectoryPath>( { path.fullPath() } );
    }

    std::optional<kpl::filesystem::DirectoryPath> DirectoryPath::make(const kpl::filesystem::DirectoryPath& dir, const RelativePath& relative_path)
    {
        return DirectoryPath::make( concatPaths(dir.absolutePath(), relative_path.path()) );
    }

    QDir makeChildDir(const kpl::Directory& parent, const QString& child_directory_name)
    {
        return QDir(parent.absolutePath() + "/" + child_directory_name);
    }

    bool fileExists(const kpl::Directory& directory, const QString& file_name)
    {
        return directory.exists(file_name);
    }

    QString withoutExtension(const QString& file_name)
    {
        u64 name_length = file_name.size();
        return kpl::forwardUntil( {file_name.data(), name_length}, '.');
    }

    QString& withoutExtensionMut(QString& file_name)
    {
        u32 name_length = file_name.size();
        u32 from_end = countElementsFromEnd({file_name.data(), name_length}, QChar{'.'});

        file_name.remove(name_length - from_end, name_length);

        return file_name;
    }

    FileIdentifier::FileIdentifier(const DirectoryPath& dir_path, const QString& file_name)
        : m_file_directory{ dir_path }, m_file_name{ file_name }
    {}

    std::optional<FileIdentifier> FileIdentifier::make(const QString& absolute_path)
    {
        QString file_name = absolute_path.split('/').back();
        QString parent_directory_path = absolute_path.chopped(absolute_path.size() - file_name.size());

        std::optional<DirectoryPath> file_directory_opt = DirectoryPath::make(parent_directory_path);

        if(file_directory_opt == std::nullopt) {
            return std::nullopt;
        }

        if(!fileExists(QDir(file_directory_opt.value().absolutePath()), file_name)) {
            return std::nullopt;
        }

        return std::optional<FileIdentifier>(FileIdentifier(file_directory_opt.value(), file_name));
    }

    std::optional<FileIdentifier> FileIdentifier::make(const DirectoryPath& dir_path, const QString& file_name)
    {
        if(!fileExists(QDir(dir_path.absolutePath()), file_name)) {
            return std::nullopt;
        }

        return std::optional<FileIdentifier>(FileIdentifier(dir_path, file_name));
    }

    QString FileIdentifier::absolutePath() const
    {
        return m_file_directory.absolutePath() + "/" + m_file_name;
    }

    QString FileIdentifier::directoryAbsolutePath() const
    {
        return m_file_directory.absolutePath();
    }

    QString FileIdentifier::rootName() const
    {
        return m_file_name.split('/').back().split('.').front();
    }

    QString FileIdentifier::fullName() const
    {
        return m_file_name;
    }

    QString FileIdentifier::extensionName() const
    {
        return m_file_name.split('.').back();
    }

    bool writeFile(const DirectoryPath& dir_path, const kpl::String& file_name, const ContinguousMemory& memory)
    {
        if(memory.isNull()) {
            return false;
        }

        kpl::String full_path = dir_path.absolutePath() + "/" + file_name;

        FILE * output_file = fopen(full_path.toUtf8(), "wb");

        if(!output_file) {
            return false;
        }

        fwrite(memory.m_memory_start, memory.m_size_bytes, 1, output_file);
        fclose(output_file);

        return true;
    }

} // End filesystem

    u32 countFilesInDirectoryRecursive(     const kpl::Directory& directory,
                                            const kpl::List<QString>& name_filters)
    {
        u32 result = 0;
        QDirIterator itr(directory.absolutePath(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while(itr.hasNext())
        {
            itr.next();
            QDir currentDir(itr.filePath());

            QStringList fileFound = currentDir.entryList(name_filters, QDir::Files, QDir::Name);
            result += fileFound.size();
        }

        return result;
    }

    QList<kpl::filesystem::FileIdentifier> findFilesInDirectoryRecursive(const QDir& directory, const QStringList& name_filters)
    {
        kpl::List<kpl::filesystem::FileIdentifier> files_found;
        QDirIterator itr(directory.absolutePath(), QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        while(itr.hasNext())
        {
            itr.next();
            QDir current_dir(itr.filePath());

            QStringList files_in_folder = current_dir.entryList(name_filters, QDir::Files, QDir::Name);

            for(const auto& file : files_in_folder)
            {
                std::optional<kpl::filesystem::FileIdentifier> file_opt = kpl::filesystem::FileIdentifier::make(current_dir, file);

                if(file_opt == std::nullopt) {
                    continue;
                }

                files_found.append(file_opt.value());
            }
        }

        return files_found;
    }

    template <typename T>
    QList<T> bestOf(const QList<T>& items, const u16 max, const std::function<bool(T,T)>& cmp)
    {
        if(items.size() <= max) {
            return items;
        }

        QList<T> result = items;
        std::sort(result.begin(), result.end(), cmp);

        result.erase(result.begin(), result.begin() + max);

        return result;
    }

//    template QList<Magick::Image> kpl::bestOf<Magick::Image>(const QList<Magick::Image>&, const u16, const std::function<bool(Magick::Image, Magick::Image)>&);

//    namespace image
//    {
//        std::optional<Magick::Image> createTiledImage(kpl::List<QString> source_images, kpl::Resolution output_resolution);

//        bool isImageValid(QString imagePath)
//        {
//            bool result = false;

//            try {
//                result = Magick::Image(imagePath.toUtf8().data()).isValid() ? true : false;
//            } catch (...) {
//                return false;
//            }

//            return result;
//        }
//    }
}
