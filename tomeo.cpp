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
#include <QFileDialog>
#include <QtCore/QDirIterator>
#include <QPropertyAnimation>
#include "record_video.h"
#include "the_player.h"
#include "the_button.h"
#include "settings.h"

// get directory from file explorer interface
QString getVideoDirectory() {
    QString dir = QFileDialog::getExistingDirectory(
        NULL,
        "Select Video Directory",
        "",
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    return dir;
}

// read in videos and thumbnails to this directory
std::vector<TheButtonInfo> getInfoIn (std::string loc) {

    //loc = "C:\\Users\\russe\\OneDrive\\Y2\\S1\\User_Interfaces\\2811_cw3-master-release-lowresbck\\videos";

    std::vector<TheButtonInfo> out =  std::vector<TheButtonInfo>();
    //qDebug() << "dir pass" <<QString::fromStdString(loc);
    QDir dir(QString::fromStdString(loc) );
    //qDebug() << dir.exists();
    //qDebug() << dir.absolutePath();
    QDirIterator it(dir);
    //qDebug() << it.next();

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
                    qDebug() << "warning: skipping video because I couldn't process thumbnail " << thumb << endl;
            }
            else
                qDebug() << "warning: skipping video because I couldn't find thumbnail " << thumb << endl;
        }
    }

    return out;
}


int main(int argc, char *argv[]) {

    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // collect all the videos in the folder
    std::vector<TheButtonInfo> videos;

    // Print out the argument passed to the application
    if (argc > 1) {
        qDebug() << "Argument passed to the application: " << argv[1] << endl;
    } else {
        qDebug() << "No argument passed to the application." << endl;
    }

    if (argc == 2)
    {
        std::cout << std::string(argv[1]) ;
        videos = getInfoIn( std::string(argv[1]) );
    }

    if (videos.empty()) {
        QMessageBox::warning(
            NULL,
            "Tomeo",
            "No videos found in the provided directory or directory doesn't exist.\nPlease select a directory with videos.");

        QString selected_dir = getVideoDirectory();
        if (!selected_dir.isEmpty()) {
        videos = getInfoIn(selected_dir.toStdString());
        }

        if (videos.empty()) {
        QMessageBox::critical(
            NULL,
            "Tomeo",
            "No videos found. The application will now exit.");
        exit(-1);
        }
    }


    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;
    videoWidget->setFixedSize(774, 900);

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);
    // tell the player what videos are available
    player->setContent(&videos);

    // create the down arrow button
    QPushButton *downArrowButton = new QPushButton("Next Video");
    downArrowButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

    // Connect the down arrow button to a slot that changes the video
    QObject::connect(downArrowButton, &QPushButton::clicked, [player](){
        player->nextVideo();
    });



    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 1040);
    // https://stackoverflow.com/questions/14943715/qwidget-reports-wrong-width-value/14944640#14944640
    //window.setFixedSize(1280,720);



    // Create a button to show the settings page
    QPushButton *settingsButton = new QPushButton("Settings", &window);
    settingsButton->setMaximumSize(100, 100);
    top->addWidget(settingsButton);

    // Create a button to show the record video page
    QPushButton *recordVideoButton = new QPushButton("Record Video", &window);
    recordVideoButton->setMaximumSize(100, 100);
    top->addWidget(recordVideoButton);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);

    // Add the down arrow button to the main window layout
    top->addWidget(downArrowButton);

    // create layout for the settings
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    Settings *settings = new Settings(&window);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->setFixedSize(400, 1000000);
    settings->setLayout(settingsLayout);
    window.setLayout(settingsLayout);

    // create layout for the record video
    QVBoxLayout *recordVideoLayout = new QVBoxLayout();
    RecordVideo *recordVideo = new RecordVideo(&window);
    recordVideo->setAttribute(Qt::WA_DeleteOnClose);
    recordVideo->hide();
    recordVideo->setMinimumSize(window.size());
    recordVideo->setLayout(recordVideoLayout);
    window.setLayout(recordVideoLayout);

    // we should generalise this animation and window setup for less code repetition.

    // Connect the button click to the animation
    QObject::connect(settingsButton, &QPushButton::clicked, [=]() {
        if (!settings->isVisible()) {
            // If settings is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(settings, "geometry");
            animation->setDuration(500); // Set the duration of the animation in milliseconds
            animation->setStartValue(settings->geometry());
            animation->setEndValue(QRect(0, 0, settings->width(), settings->height()));
            animation->start();
            settings->show();
            settings->raise();
        } else {
            // If settings is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(settings, "geometry");
            animation->setDuration(500);
            animation->setStartValue(settings->geometry());
            animation->setEndValue(QRect(-settings->width(), 0, settings->width(), settings->height()));
            animation->start();

            QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
                settings->hide();
            });
        }
    });

    // Connect the record_video button click to the animation
    QObject::connect(recordVideoButton, &QPushButton::clicked, [=]() {
        if (!recordVideo->isVisible()) {
            // If settings is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(recordVideo, "geometry");
            animation->setDuration(500); // Set the duration of the animation in milliseconds
            animation->setStartValue(recordVideo->geometry());
            animation->setEndValue(QRect(0, 0, recordVideo->width(), recordVideo->height()));
            animation->start();
            recordVideo->show();
            recordVideo->raise();
            recordVideoButton->raise(); // currently just raising the button to easily close the layout
        } else {
            // If settings is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(recordVideo, "geometry");
            animation->setDuration(500);
            animation->setStartValue(recordVideo->geometry());
            animation->setEndValue(QRect(-recordVideo->width(), 0, recordVideo->width(), recordVideo->height()));
            animation->start();

            QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
                recordVideo->hide();
            });
        }
    });

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
