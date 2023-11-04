#include "editingbuttons.h"

EditingButtons::EditingButtons(QWidget* parent): QHBoxLayout(parent)
{
    QPushButton *add_playlist = new QPushButton("Add to Playlist");
    QPushButton *delete_playlist = new QPushButton("Delete Selected");
    QPushButton *search = new QPushButton("Search");
    addWidget(add_playlist);
    addWidget(delete_playlist);
    addWidget(search);
}
