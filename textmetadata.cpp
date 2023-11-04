#include "textmetadata.h"

#include <QLineEdit>
#include <QLabel>

TextMetadata::TextMetadata(QWidget* parent): QVBoxLayout(parent)
{
    QLineEdit* playlistTitle = new QLineEdit;
    playlistTitle->setPlaceholderText("Title");

    QLineEdit* username = new QLineEdit;
    username->setPlaceholderText("Username");

    QLabel* length = new QLabel("Length: ");

    addWidget(playlistTitle);
    addWidget(username);
    addWidget(length);
}
