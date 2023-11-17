#ifndef TEXTMETADATA_H
#define TEXTMETADATA_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class Playlist;

class TextMetadata: public QVBoxLayout
{
public:
    TextMetadata(Playlist* playlist);
    void updateDurationLabel(qint64 duration);

private:
    QLabel* duration;
};

#endif // TEXTMETADATA_H
