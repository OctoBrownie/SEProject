#include<QWidget>
#include<QPushButton>

#include "applicationbuttons.h"

ApplicationButtons::ApplicationButtons(QWidget* parent): QHBoxLayout(parent)
{
    QPushButton *import_button = new QPushButton("Import Playlist");
    QPushButton *save_button = new QPushButton("Save Playlist");
    QPushButton *export_button = new QPushButton("Export Playlist");
    QPushButton *settings_button = new QPushButton("Settings");
    addWidget(import_button);
    addWidget(save_button);
    addWidget(export_button);
    addWidget(settings_button);
}
