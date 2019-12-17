#include <kpl/audiofile.h>

namespace kpl {
namespace audio {

    QString tagToQString(TagLib::String string)
    {
        QString result;
        result.reserve(string.size());

        for(auto itr = string.begin(); itr != string.end(); itr++) {
            result.append( *itr );
        }

        return result;
    }

    QString tagToQString(TagLib::String& string)
    {
        QString result;
        result.reserve(string.size());

        for(auto itr = string.begin(); itr != string.end(); itr++) {
            result.append( QChar{*itr} );
        }

        return result;
    }

    kpl::String extractTitle(kpl::filesystem::FileIdentifier file)
    {
        return  extractTitle(extractTag(file));
    }

    kpl::String extractArtist(kpl::filesystem::FileIdentifier file)
    {
        return  extractArtist(extractTag(file));
    }

    kpl::String extractAlbum(kpl::filesystem::FileIdentifier file)
    {
        return  extractAlbum(extractTag(file));
    }

    kpl::String extractGenre(kpl::filesystem::FileIdentifier file)
    {
        return  extractGenre(extractTag(file));
    }

    i32 extractReleaseYear(kpl::filesystem::FileIdentifier file)
    {
        return extractReleaseYear(extractTag(file));
    }

    i32 extractTrackNumber(kpl::filesystem::FileIdentifier file)
    {
        return  extractTrackNumber(extractTag(file));
    }

    TagLib::Tag* extractTag(kpl::filesystem::FileIdentifier file)
    {
        TagLib::FileRef f(file.absolutePath().toUtf8());
        return f.tag();
    }

    kpl::String extractTitle(TagLib::Tag *tag)
    {
        return tagToQString(tag->title());
    }

    kpl::String extractArtist(TagLib::Tag *tag)
    {
        return tagToQString(tag->artist());
    }

    kpl::String extractAlbum(TagLib::Tag *tag)
    {
        return tagToQString(tag->album());
    }

    kpl::String extractGenre(TagLib::Tag *tag)
    {
        return tagToQString(tag->genre());
    }

    i32 extractReleaseYear(TagLib::Tag *tag)
    {
        return tag->year();
    }

    i32 extractTrackNumber(TagLib::Tag *tag)
    {
        return tag->track();
    }

    i32 extractLengthSeconds(kpl::filesystem::FileIdentifier file)
    {
        TagLib::FileRef file_ref(file.absolutePath().toUtf8());

        if(file_ref.isNull()) {
            return -1;
        }

        TagLib::AudioProperties * properties = file_ref.audioProperties();

        if(!properties) {
            return -1;
        }

        return properties->lengthInSeconds();
    }

    ContinguousMemory extractAlbumArt(kpl::filesystem::FileIdentifier file)
    {
        static const char *id_picture = "APIC" ;

        TagLib::MPEG::File mpeg_file(file.absolutePath().toUtf8());
        TagLib::ID3v2::Tag *id3v2_tag = mpeg_file.ID3v2Tag();
        TagLib::ID3v2::FrameList frames;
        TagLib::ID3v2::AttachedPictureFrame *picture_frame;

        void *image_data;
        unsigned long image_size_bytes;

        if (id3v2_tag)
        {
            frames = id3v2_tag->frameListMap()[id_picture] ;

            if (!frames.isEmpty() )
            {
                for(TagLib::ID3v2::FrameList::ConstIterator it = frames.begin(); it != frames.end(); ++it)
                {
                    picture_frame = (TagLib::ID3v2::AttachedPictureFrame *)(*it);

                    if(picture_frame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover)
                    {
                        image_size_bytes = picture_frame->picture().size();
                        image_data = malloc(image_size_bytes);
                        memcpy(image_data, picture_frame->picture().data(), image_size_bytes);

                        if(image_data) {
                            return ContinguousMemory(reinterpret_cast<u8*>(image_data), image_size_bytes);
                        }
                    }
                }
            }
        }

        return ContinguousMemory::nullMemory();
    }

}
}
