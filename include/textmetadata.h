#ifndef TEXTMETADATA_H
#define TEXTMETADATA_H

#include<QVBoxLayout>

class QLineEdit;
class QLabel;

//Text Metadata class. Displays GUI for playlist metadata
class TextMetadata: public QVBoxLayout
{
public:
    // Constructor
    TextMetadata(QString pName, QString uName, qint64 duration);

    // Getters
    QLineEdit* getPlaylistTitle();
    QLineEdit* getUsername();

    void updateDurationLabel(qint64 duration);

private:
    //GUI element that edits playlist name
    QLineEdit* playlistName;

    //GUI element that edits users name
    QLineEdit* userName;

    //GUI element that displays the duration
    QLabel* duration;
};

#endif // TEXTMETADATA_H
