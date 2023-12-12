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
#include "friends.h"
#include "faqlayout.h"

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
                    if(!f.contains("user_video")){ // dont add the user video initially
                        out . push_back(TheButtonInfo( url , ico  ) ); // add to the output list
                    }
                    else{
                        // the user file is always the 0th in the vector
                        out . insert(out.begin(), TheButtonInfo( url , ico  ) ); // add to the front of the output list
                    }
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

    QString buildPath = QCoreApplication::applicationDirPath();

    // Check if the platform is macOS
    qDebug() << QSysInfo::productType();
    if (QSysInfo::productType() == "osx") {
        // If macOS, remove the contents part of the build path
        buildPath = buildPath.left(buildPath.lastIndexOf("/the.app"));
    }
    qDebug() << buildPath;
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
    videoWidget->setFixedSize(774, 700);

    QPushButton *upArrowButton = new QPushButton(videoWidget);
    QString upIconFilename = "up_arrow.png";
    QString upIconPath = QDir(buildPath).filePath("icons/" + upIconFilename);
    qDebug() << upIconPath << Qt::endl;
    upArrowButton->setIcon(QIcon(upIconPath));
    upArrowButton->setIconSize(QSize(70, 30));
    upArrowButton->setFlat(true);
    //upArrowButton->setGeometry(250, 10, 70, 30);
    // create the down arrow button
    QPushButton *downArrowButton = new QPushButton(videoWidget);
    QString downIconFilename = "down_arrow.png";
    QString downIconPath = QDir(buildPath).filePath("icons/" + downIconFilename);
    downArrowButton->setIcon(QIcon(downIconPath));
    downArrowButton->setIconSize(QSize(70, 30));
    downArrowButton->setFlat(true);
    //downArrowButton->setGeometry(250, 860, 70, 30);


    int videoWidgetWidth = 750;
    int buttonHeight = 70; // Set the button height as needed
    // Position the up arrow button at the top of the video widget
    upArrowButton->setGeometry(0, 10, videoWidgetWidth, buttonHeight);

    // Position the down arrow button at the bottom of the video widget
    // If the video widget is 900px high, the y position would be 900 - buttonHeight
    downArrowButton->setGeometry(0, 1080 - buttonHeight, videoWidgetWidth, buttonHeight);


    // create the main window and layout
    QWidget window;
    QVBoxLayout *top = new QVBoxLayout();
    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 1080);
    // https://stackoverflow.com/questions/14943715/qwidget-reports-wrong-width-value/14944640#14944640
    //window.setFixedSize(1280,720);


    QPushButton *settingsButton = new QPushButton(videoWidget);
    QString settingsIconFilename = "settings.png";
    QString settingsIconPath = QDir(buildPath).filePath("icons/" + settingsIconFilename);
    settingsButton->setIcon(QIcon(settingsIconPath));
    settingsButton->setIconSize(QSize(50, 50));
    settingsButton->setFlat(true);
    settingsButton->setGeometry(10, 10, 50, 50);


    QPushButton *profileButton = new QPushButton(videoWidget);
    QString profileIconFilename = "profile_icon.png";
    QString profileIconPath = QDir(buildPath).filePath("icons/" + profileIconFilename);
    profileButton->setIcon(QIcon(profileIconPath));
    profileButton->setIconSize(QSize(50, 50));
    profileButton->setFlat(true);
    profileButton->setGeometry(10, 850, 50, 50);


    QPushButton *recordVideoButton = new QPushButton;
    QString recordVideoIconFilename = "camera.png";
    QString recordVideoIconPath = QDir(buildPath).filePath("icons/" + recordVideoIconFilename);
    recordVideoButton->setIcon(QIcon(recordVideoIconPath));
    recordVideoButton->setIconSize(QSize(50, 50));
    recordVideoButton->setFlat(true);
    recordVideoButton->setGeometry(10, 10, 50, 50);

    QPushButton *faqButton = new QPushButton;
    QString faqIconFilename = "faq.png";
    QString faqIconPath = QDir(buildPath).filePath("icons/" + faqIconFilename);
    faqButton->setIcon(QIcon(faqIconPath));
    faqButton->setIconSize(QSize(45, 45)); // smaller icon size
    faqButton->setFlat(true);

    QLabel *faqTextLabel = new QLabel("F.A.Q");
    faqTextLabel->setStyleSheet("color: black; font-size: 12px;");

    QHBoxLayout *faqButtonLayout = new QHBoxLayout;
    faqButtonLayout->addWidget(faqTextLabel);
    faqButtonLayout->addWidget(faqButton);
    faqButtonLayout->setAlignment(Qt::AlignRight | Qt::AlignBottom);

    QHBoxLayout *navbar;
    navbar = new QHBoxLayout;
    navbar->addWidget(settingsButton);
    navbar->addWidget(recordVideoButton);
    navbar->addWidget(profileButton);

    navbar->setSpacing(100);
    navbar->setAlignment(Qt::AlignTop);
    top->addLayout(navbar);


    // add the video and the buttons to the top level widget
    top->addWidget(upArrowButton);
    // create layout for the record video
    QVBoxLayout *recordVideoLayout = new QVBoxLayout();
    RecordVideo *recordVideo = new RecordVideo(&window);
    recordVideo->setAttribute(Qt::WA_DeleteOnClose);
    recordVideo->hide();
    recordVideo->setMinimumSize(window.size());
    recordVideo->setLayout(recordVideoLayout);
    window.setLayout(recordVideoLayout);



    // the QMediaPlayer which controls the playback
    ThePlayer *player = new ThePlayer(recordVideo);
    player->setVideoOutput(videoWidget);
    // Connect the down arrow button to a slot that changes the video
    QObject::connect(upArrowButton, &QPushButton::clicked, [player]() { player->previousVideo(); });
    QObject::connect(downArrowButton, &QPushButton::clicked, [player]() { player->nextVideo(); });
    // tell the player what videos are available
    player->setContent(&videos);
    top->addWidget(videoWidget);

    // Add the down arrow button to the main window layout
    top->addWidget(downArrowButton);
    top->addLayout(faqButtonLayout);

    //create layout for the settings
    QVBoxLayout *friendsLayout = new QVBoxLayout();
    Friends *friends = new Friends(&window);
    friends->setAttribute(Qt::WA_DeleteOnClose);
    friends->hide();
    friends->setMinimumSize(window.size());
    friends->setLayout(friendsLayout);

    QVBoxLayout *faqLayout = new QVBoxLayout();
    FaqLayout *faq = new FaqLayout(&window);
    faq->setAttribute(Qt::WA_DeleteOnClose);
    faq->hide();
    faq->setMinimumSize(window.size());
    faq->setLayout(faqLayout);

    // create layout for the settings
    QVBoxLayout *settingsLayout = new QVBoxLayout();
    Settings *settings = new Settings(&window);
    settings->setAttribute(Qt::WA_DeleteOnClose);
    settings->setFixedSize(400, 1080);
    settings->setLayout(settingsLayout);
    window.setLayout(settingsLayout);

    QPalette newPalette;
    newPalette.setColor(QPalette::Background, QColor("#F8FFF4"));
    window.setStyleSheet("color: black;");
    qDebug() << "Set theme to light";
    recordVideo->setAutoFillBackground(true);
    recordVideo->setPalette(newPalette);
    friends->setAutoFillBackground(true);
    friends->setPalette(newPalette);
    window.setAutoFillBackground(true);
    window.setPalette(newPalette);

    QObject::connect(settings, &Settings::themeChanged, [&window, &recordVideo, &friends](const QString& theme) {
        QPalette newPalette;

        if (theme == "Dark Mode") {
            newPalette.setColor(QPalette::Background, QColor("#1D1C21"));
            window.setStyleSheet("color: #9c9c9c;");
            qDebug() << "Set theme to dark";
        } else if (theme == "Light Mode") {
            newPalette.setColor(QPalette::Background, QColor("#F8FFF4"));
            window.setStyleSheet("color: black;");
            qDebug() << "Set theme to light";
        }

        recordVideo->setAutoFillBackground(true);
        recordVideo->setPalette(newPalette);
        friends->setAutoFillBackground(true);
        friends->setPalette(newPalette);
        window.setPalette(newPalette);
        window.setAutoFillBackground(true);
    });



    // we should generalise this animation and window setup for less code repetition.

    // Connect the button click to the animation
    QObject::connect(settingsButton, &QPushButton::clicked, [=]() {
        if (!settings->isVisible()) {
            // If settings is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(settings, "geometry");
            animation->setDuration(100); // Set the duration of the animation in milliseconds
            animation->setStartValue(settings->geometry());
            animation->setEndValue(QRect(0, 0, settings->width(), settings->height()));
            animation->start();
            recordVideo->hide();
            friends->hide();
            settings->show();
            settings->raise();
        } else {
            // If settings is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(settings, "geometry");
            animation->setDuration(100);
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
            // If recordVideo is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(recordVideo, "geometry");
            animation->setDuration(100);
            animation->setStartValue(QRect(0, -recordVideo->height(), recordVideo->width(), recordVideo->height()));
            animation->setEndValue(QRect(0, 0, recordVideo->width(), recordVideo->height()));
            animation->start();
            settings->hide();
            friends->hide();
            recordVideo->show();
            recordVideoButton->setIcon(QIcon(QDir(buildPath).filePath("icons/cross.png")));
            recordVideo->raise();
            recordVideoButton->raise();
        } else {
            // If recordVideo is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(recordVideo, "geometry");
            animation->setDuration(100);
            animation->setStartValue(recordVideo->geometry());
            animation->setEndValue(QRect(0, -recordVideo->height(), recordVideo->width(), recordVideo->height()));
            animation->start();
            recordVideoButton->setIcon(QIcon(recordVideoIconPath));

            QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
                recordVideo->hide();
            });
        }
    });

    // Connect the friends button click to the animation
    QObject::connect(profileButton, &QPushButton::clicked, [=]() {
        if (!friends->isVisible()) {
            // If recordVideo is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(friends, "geometry");
            animation->setDuration(100);
            animation->setStartValue(QRect(0, -friends->height(), friends->width(), friends->height()));
            animation->setEndValue(QRect(0, 0, friends->width(), friends->height()));
            animation->start();
            settings->hide();
            recordVideo->hide();
            friends->show();
            friends->raise();
            profileButton->setIcon(QIcon(QDir(buildPath).filePath("icons/cross.png")));
            profileButton->raise();
        } else {
            // If recordVideo is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(friends, "geometry");
            animation->setDuration(100);
            animation->setStartValue(friends->geometry());
            animation->setEndValue(QRect(0, -friends->height(), friends->width(), friends->height()));
            animation->start();
            profileButton->setIcon(QIcon(profileIconPath));

            QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
                friends->hide();
            });
        }
    });

    QObject::connect(faqButton, &QPushButton::clicked, [=]() {
        if (!faq->isVisible()) {
            // If recordVideo is not visible, show it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(faq, "geometry");
            animation->setDuration(100);
            animation->setStartValue(QRect(0, -faq->height(), faq->width(), faq->height()));
            animation->setEndValue(QRect(0, 0, faq->width(), faq->height()));
            animation->start();
            settings->hide();
            faq->hide();
            faq->show();
            faq->raise();
            faqButton->setIcon(QIcon(QDir(buildPath).filePath("icons/cross.png")));
            faqButton->raise();
        } else {
            // If recordVideo is visible, hide it with animation
            QPropertyAnimation *animation = new QPropertyAnimation(faq, "geometry");
            animation->setDuration(100);
            animation->setStartValue(friends->geometry());
            animation->setEndValue(QRect(0, -faq->height(), faq->width(), faq->height()));
            animation->start();
            faqButton->setIcon(QIcon(faqIconPath));

            QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
                faq->hide();
            });
        }
    });

    // showtime!
    window.show();

    // wait for the app to terminate
    return app.exec();
}
