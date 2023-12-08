//
//    ______
//   /_  __/___  ____ ___  ___  ____
//    / / / __ \/ __ `__ \/ _ \/ __ \
//   / / / /_/ / / / / / /  __/ /_/ /
//  /_/  \____/_/ /_/ /_/\___/\____/
//              video for sports enthusiasts...
//
//

#include <iostream>
#include <QApplication>
#include <QtMultimediaWidgets/QVideoWidget>
#include <QMediaPlaylist>
#include <string>
#include <vector>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
#include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include "the_player.h"
#include "the_button.h"

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    QDir dir(QString::fromStdString(loc) );
    QDirIterator it(dir);

    while (it.hasNext()) { // for all files

        QString f = it.next();

        if (f.contains("."))

#if defined(_WIN32)
        if (f.contains(".wmv"))  { // windows
#else
        if (f.contains(".mp4") || f.contains("MOV"))  { // mac/linux
#endif

            QString thumb = f.left( f .length() - 4) +".png";
            if (QFile(thumb).exists()) { // if a png thumbnail exists
                QImageReader *imageReader = new QImageReader(thumb);
                QImage sprite = imageReader->read(); // read the thumbnail
                if (!sprite.isNull()) {
                    QIcon* ico = new QIcon(QPixmap::fromImage(sprite)); // voodoo to create an icon for the button
                    QUrl* url = new QUrl(QUrl::fromLocalFile( f )); // convert the file location to a generic url
                    out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                }
                else
                    qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb <<Qt::endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << Qt::endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR <<Qt::endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    if (argc == 2)
        videos = getInfoIn( std::string(argv[1]) );

    if (videos.size() == 0) {

        const int result = QMessageBox::information(
            NULL,
            QString("Tomeo"),
            QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // Create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("Tomeo");
    window.setMinimumSize(540, 960);
    window.setMaximumSize(540, 960);

    // The widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    videoWidget->setFixedSize(500, 900);
    top->addWidget(videoWidget);

    // The QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);
    player->setContent(&videos);

    // Overlay buttons on the video
    QPushButton *upArrowButton = new QPushButton(videoWidget);
    upArrowButton->setIcon(QIcon("/path/to/up_arrow.png"));  // Replace with actual path
    upArrowButton->setIconSize(QSize(70, 30));
    upArrowButton->setFlat(true);
    upArrowButton->setGeometry(0, 10, 500, 50); // Full width

    QPushButton *downArrowButton = new QPushButton(videoWidget);
    downArrowButton->setIcon(QIcon("/path/to/down_arrow.png"));  // Replace with actual path
    downArrowButton->setIconSize(QSize(70, 30));
    downArrowButton->setFlat(true);
    downArrowButton->setGeometry(0, 900 - 50, 500, 50); // Full width

    // Connect buttons to their slots
    QObject::connect(upArrowButton, &QPushButton::clicked, [player]() { player->previousVideo(); });
    QObject::connect(downArrowButton, &QPushButton::clicked, [player]() { player->nextVideo(); });

    // Settings button
    QPushButton *settingsButton = new QPushButton(videoWidget);
    settingsButton->setIcon(QIcon("/Users/emersonsmac/OneDrive - University of Adelaide/2023/Leeds stuff/Classes/UI/2811-ui-cwk/settings.png"));
    settingsButton->setIconSize(QSize(50, 50));
    settingsButton->setFlat(true);
    settingsButton->setGeometry(10, 10, 50, 50);

    // Camera button
    QPushButton *cameraButton = new QPushButton(videoWidget);
    cameraButton->setIcon(QIcon("/Users/emersonsmac/OneDrive - University of Adelaide/2023/Leeds stuff/Classes/UI/2811-ui-cwk/settings.png"));  // Replace with actual path
    cameraButton->setIconSize(QSize(50, 50)); // Adjust size as needed
    cameraButton->setFlat(true);
    cameraButton->setGeometry((500 - 50) / 2, 10, 50, 50); // Centered at the top of videoWidget

    // Settings button (Add Friends icon)
    QPushButton *friendsButton = new QPushButton(videoWidget);
    friendsButton->setIcon(QIcon("/Users/emersonsmac/OneDrive - University of Adelaide/2023/Leeds stuff/Classes/UI/2811-ui-cwk/settings.png"));
    friendsButton->setIconSize(QSize(50, 50)); // Adjust size as needed
    friendsButton->setFlat(true);
    friendsButton->setGeometry(10, 10, 50, 50); // Top left corner

    // Profile button
    QPushButton *profileButton = new QPushButton(videoWidget);
    profileButton->setIcon(QIcon("/Users/emersonsmac/OneDrive - University of Adelaide/2023/Leeds stuff/Classes/UI/2811-ui-cwk/settings.png"));
    profileButton->setIconSize(QSize(50, 50));
    profileButton->setFlat(true);
    profileButton->setGeometry(10, 850, 50, 50);

    // Window setup
    window.show();

    // Run the application
    return app.exec();
}
