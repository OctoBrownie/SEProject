#include "customsongwidget.h"

CustomSongWidget::CustomSongWidget(QString* title, QString* artist, QString* album, QImage* albumArt): QWidget()
{
    QHBoxLayout* outerLayout = new QHBoxLayout();
    QVBoxLayout* innerLayout = new QVBoxLayout();
    QLabel* titleText = new QLabel(*title);
    QLabel* artistText = new QLabel(*artist);
    QLabel* albumTest = new QLabel(*album);

    innerLayout->addWidget(titleText);
    innerLayout->addWidget(artistText);
    innerLayout->addWidget(albumTest);

    QLabel* art = new QLabel();
    QSize changedSize(100,100);
    QPixmap map;

    if (!albumArt->isNull()) {
        map = QPixmap::fromImage(*albumArt).scaled(changedSize, Qt::KeepAspectRatio);
    } else {
        QPixmap defaultImage(":/resources/images/DefaultMusicImage.png");
        map = defaultImage.scaled(changedSize, Qt::KeepAspectRatio);
    }
    art->setPixmap(map);

    QSpacerItem* spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

    outerLayout->addWidget(art);
    outerLayout->addSpacerItem(spacer);
    outerLayout->addLayout(innerLayout);
    outerLayout->setAlignment(Qt::AlignLeft);
    setLayout(outerLayout);
}

void CustomSongWidget::mousePressEvent(QMouseEvent *event) {
    qDebug() << "Widget Clicked!!";
}
