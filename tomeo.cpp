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
#include <QPropertyAnimation>
#include "the_player.h"
#include "the_button.h"
#include "settings.h"

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

    if (videos.size() == 0) {

        //const int result = QMessageBox::information( swapped to remove error message
        QMessageBox::information(
                    NULL,
                    QString("Tomeo"),
                    QString("no videos found! Add command line argument to \"quoted\" file location."));
        exit(-1);
    }

    // the widget that will show the video
    QVideoWidget *videoWidget = new QVideoWidget;

    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer;
    player->setVideoOutput(videoWidget);

    // a row of buttons
    QWidget *buttonWidget = new QWidget();
    // a list of the buttons
    std::vector<TheButton*> buttons;
    // the buttons are arranged horizontally
    QHBoxLayout *layout = new QHBoxLayout();
    buttonWidget->setLayout(layout);


    // create the four buttons
    for ( int i = 0; i < 4; i++ ) {
        TheButton *button = new TheButton(buttonWidget);
        button->connect(button, SIGNAL(jumpTo(TheButtonInfo* )), player, SLOT (jumpTo(TheButtonInfo*))); // when clicked, tell the player to play.
        buttons.push_back(button);
        layout->addWidget(button);
        button->init(&videos.at(i));
    }

    // tell the player what buttons and videos are available
    player->setContent(&buttons, & videos);

    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    // Create a button to show the settings page
    QPushButton *settingsButton = new QPushButton("Settings", &window);
    settingsButton->setMaximumSize(100, 100);
    top->addWidget(settingsButton);

    // add the video and the buttons to the top level widget
    top->addWidget(videoWidget);
    top->addWidget(buttonWidget);

    // create layout for the settings
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    Settings *settings = new Settings(&window);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->setFixedSize(400, 1000000);
    settings->setLayout(settingsLayout);
    window.setLayout(settingsLayout);

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

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
