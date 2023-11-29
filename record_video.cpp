#include "QtMultimedia/qmediarecorder.h"
#include <QApplication>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QVideoEncoderSettings>
#include <QtMultimedia/QCamera>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>
#include <iostream>
#include <QDebug>
//#include <iostream>
//#include <QApplication>
//#include <QtMultimediaWidgets/QVideoWidget>
//#include <QMediaPlaylist>
#include <string>
#include <vector>
//#include <QtWidgets/QPushButton>
//#include <QtWidgets/QHBoxLayout>
//#include <QtCore/QFileInfo>
#include <QtWidgets/QFileIconProvider>
#include <QDesktopServices>
#include <QImageReader>
//include <QMessageBox>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
//#include "the_player.h"
//#include "the_button.h"

int main(int argc, char *argv[]) {
    // let's just check that Qt is operational first
    qDebug() << "Qt version: " << QT_VERSION_STR << endl;

    // create the Qt Application
    QApplication app(argc, argv);

    // Print out the argument passed to the application
    if (argc > 1) {
        qDebug() << "Argument passed to the application: " << argv[1] << endl;
    } else {
        qDebug() << "No argument passed to the application." << endl;
    }

    if (argc == 2) {
        std::cout << std::string(argv[1]);
    }

    qDebug() << "before camera setup" << endl;
    // the widget that will show the camera feed
    QCameraViewfinder *viewfinder = new QCameraViewfinder;
    QCamera *camera = new QCamera;
    QMediaRecorder *recorder = new QMediaRecorder(camera);
    camera->setViewfinder(viewfinder);

    QVideoEncoderSettings settings = recorder->videoSettings();
    settings.setQuality(QMultimedia::VeryHighQuality);
    settings.setFrameRate(30.0);

    recorder->setVideoSettings(settings);
    camera->setCaptureMode(QCamera::CaptureVideo);
    //camera->start();

    qDebug() << "setting up layout" << endl;
    // create the main window and layout
    QWidget window;
    QHBoxLayout *top = new QHBoxLayout();
    // two QHBoxLayouts in the QVBox to make two columns.
    QVBoxLayout *left_side = new QVBoxLayout();
    QVBoxLayout *right_side = new QVBoxLayout();
    // in the left side are two verticals, one for the event info above the viewfinder
    QVBoxLayout *event_info_box = new QVBoxLayout();
    // in the right side are just some buttons no more boxes

    // event_info box
    QLabel *label = new QLabel("Text");

    // Set the maximum height for the event_info_box
    label->setMaximumHeight(20);

    QPushButton *searchButton = new QPushButton("&Search");
    QPushButton *closeButton = new QPushButton("Close");

    // Set stylesheet to add outlines
    label->setStyleSheet("border: 1px solid blue;");
    searchButton->setStyleSheet("border: 1px solid yellow;");
    closeButton->setStyleSheet("border: 1px solid purple;");

    // Set the layout policies for event_info_box
    event_info_box->setAlignment(Qt::AlignTop); // Align to the top of the column
    event_info_box->addWidget(label);

    left_side->addLayout(event_info_box);
    left_side->addWidget(viewfinder);

    right_side->addWidget(searchButton);
    right_side->addWidget(closeButton);

    top->addLayout(left_side);
    top->addLayout(right_side);

    window.setLayout(top);
    window.setWindowTitle("tomeo");
    window.setMinimumSize(800, 680);

    qDebug() << "Before window show" << endl;

    // showtime!
    window.show();

    // wait for the app to terminate
    qDebug() << "Exec" << endl;
    return app.exec();
}
