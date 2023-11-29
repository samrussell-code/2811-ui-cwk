#include "record_video.h"
#include <iostream>
#include <QDebug>

RecordVideo::RecordVideo(QWidget *parent) : QMainWindow(parent)
{
    setupCamera();

    // create the main window and layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(centralWidget);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    eventInfoLayout = new QVBoxLayout();

    // event_info box
    label = new QLabel("Text");
    label->setMaximumHeight(20);
    label->setStyleSheet("border: 1px solid blue;");
    eventInfoLayout->setAlignment(Qt::AlignTop);
    eventInfoLayout->addWidget(label);

    leftLayout->addLayout(eventInfoLayout);
    leftLayout->addWidget(viewfinder);

    searchButton = new QPushButton("Record");
    closeButton = new QPushButton("Flip");
    searchButton->setStyleSheet("border: 1px solid yellow;");
    closeButton->setStyleSheet("border: 1px solid purple;");

    rightLayout->addWidget(searchButton);
    rightLayout->addWidget(closeButton);

    topLayout->addLayout(leftLayout);
    topLayout->addLayout(rightLayout);

    centralWidget->setLayout(topLayout);
    setCentralWidget(centralWidget);

    setWindowTitle("tomeo");
    setMinimumSize(800, 680);
}

RecordVideo::~RecordVideo()
{
    camera->stop();
}

void RecordVideo::setupCamera()
{
    // the widget that will show the camera feed
    viewfinder = new QCameraViewfinder(this);
    camera = new QCamera(this);
    recorder = new QMediaRecorder(camera);
    camera->setViewfinder(viewfinder);

    QVideoEncoderSettings videoSettings = recorder->videoSettings();
    videoSettings.setQuality(QMultimedia::VeryHighQuality);
    videoSettings.setFrameRate(30.0);

    recorder->setVideoSettings(videoSettings);
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}
