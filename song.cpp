#include "song.h"

Song::Song(const char* path)
{
    this->songPath = new QString(path);

    TagLib::MPEG::File file(path);

    if (file.hasID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = file.ID3v2Tag();

        this->title = QString::fromStdString(tag->title().to8Bit(true));
        this->artists = QString::fromStdString(tag->artist().to8Bit(true));
        this->album = QString::fromStdString(tag->album().to8Bit(true));
        this->duration = file.audioProperties()->lengthInSeconds();
        TagLib::ID3v2::FrameList frameList = tag->frameList("APIC");

        if (!frameList.isEmpty()) {
            TagLib::ID3v2::AttachedPictureFrame *pictureFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frameList.front());

            if (pictureFrame) {
                QImage albumArt;
                albumArt.loadFromData(reinterpret_cast<const uchar*>(pictureFrame->picture().data()), pictureFrame->picture().size());
                this->albumArt = albumArt;
            }
        }
    }
    createSongBox();
}

void Song::createSongBox() {
    CustomSongWidget* widget = new CustomSongWidget(&this->title, &this->artists, &this->album, &this->albumArt);
    this->songBox = widget;
}

QString* Song::getSongPath() {
    return this->songPath;
}

qint64 Song::getDuration() {
    return this->duration;
}


qint64 Song::getPosition() {
    return this->pPosition;
}
void Song::setPosition(qint64 newPosition) {
    this->pPosition = newPosition;
}

CustomSongWidget* Song::getBox() {
    return this->songBox;
}

