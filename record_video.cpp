// record_video.cpp

#include "record_video.h"
#include "qcamerainfo.h"
#include "qdatetime.h"
#include "qtimer.h"
#include "random_time_generator.h"
#include <QDir>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QPalette>
#include <QApplication>
#include <QList>
#include <QDebug>

RecordVideo::RecordVideo(QWidget *parent) : QWidget(parent)
{
    setupCamera();

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(centralWidget);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    eventInfoLayout = new QVBoxLayout();
    this->setAutoFillBackground(true); // background fill


    // create the generator that will set the initial label time.
    RandomTimeGenerator *gen = new RandomTimeGenerator;
    schedule_time = new QLabel(gen->scheduleTime());
    schedule_time->setMaximumHeight(20);
    eventInfoLayout->setAlignment(Qt::AlignTop);
    eventInfoLayout->addWidget(schedule_time);

    leftLayout->addLayout(eventInfoLayout);
    leftLayout->addWidget(viewfinder);

    // create and start a timer to update the schedule time every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RecordVideo::updateScheduleTime);
    timer->start(1000);

    QString buildPath = QApplication::applicationDirPath();
    QString recordIconFilename = "record.png";
    QString flipIconFilename = "flip_dark.png";
    QString recordIconPath = QDir(buildPath).filePath("icons/" + recordIconFilename);
    QString flipIconPath = QDir(buildPath).filePath("icons/" + flipIconFilename);
    qDebug() << recordIconPath;
    QPushButton *recordButton = new QPushButton(QIcon(recordIconPath), "");
    QPushButton *flipButton = new QPushButton(QIcon(flipIconPath), "");

    cameraComboBox = new QComboBox();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "Name: " << cameraInfo.description();
        cameraComboBox->addItem(cameraInfo.description(), QVariant::fromValue(cameraInfo));
    }

    connect(cameraComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index) { switchCamera(index); });

    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
    recordButton->setStyleSheet(buttonStyle); // Style sheet
    recordButton->setIconSize(QSize(75, 75));
    recordButton->setMinimumSize(75, 75);
    recordButton->setMaximumSize(75, 75);

    flipButton->setStyleSheet(buttonStyle); // Style sheet
    flipButton->setIconSize(QSize(75, 75));
    flipButton->setMinimumSize(75, 75);
    flipButton->setMaximumSize(75, 75);

    QPushButton *toggleModeButton = new QPushButton("Toggle Mode");
    connect(toggleModeButton, &QPushButton::clicked, this, &RecordVideo::toggleRecordingMode);

    rightLayout->addWidget(cameraComboBox);
    rightLayout->addWidget(flipButton);
    rightLayout->addWidget(recordButton);
    rightLayout->addWidget(toggleModeButton);

    topLayout->addLayout(leftLayout);
    topLayout->addLayout(rightLayout);

    setLayout(topLayout);

    setMinimumSize(774, 1040);
}

RecordVideo::~RecordVideo()
{
    camera->stop();
}

void RecordVideo::setupCamera()
{
    viewfinder = new QCameraViewfinder(this);

    camera = new QCamera(this);
    recorder = new QMediaRecorder(camera);
    camera->setViewfinder(viewfinder);

    QVideoEncoderSettings videoSettings = recorder->videoSettings();
    videoSettings.setQuality(QMultimedia::NormalQuality);
    videoSettings.setFrameRate(30.0);

    recorder->setVideoSettings(videoSettings);
    camera->setCaptureMode(QCamera::CaptureVideo);
    camera->start();
}

void RecordVideo::toggleRecordingMode()
{
    verticalMode = !verticalMode;

    updateCameraSettings();
    updateViewfinderSettings();

    camera->stop();
    camera->start();
}

void RecordVideo::updateCameraSettings()
{
    QVideoEncoderSettings videoSettings = recorder->videoSettings();

    if (verticalMode)
    {
        videoSettings.setResolution(720, 1280);
    }
    else
    {
        videoSettings.setResolution(1280, 720);
    }

    recorder->setVideoSettings(videoSettings);
}

void RecordVideo::updateViewfinderSettings()
{
    QCameraViewfinderSettings viewfinderSettings = camera->viewfinderSettings();
    QList<QSize> availableSettings = camera->supportedViewfinderResolutions();

    qDebug() << "Supported Viewfinder Resolutions:";
    for (const QSize &resolution : availableSettings)
    {
        qDebug() << "Resolution: " << resolution;

        viewfinderSettings.setResolution(resolution);

        qDebug() << "Pixel Aspect Ratio: " << viewfinderSettings.pixelAspectRatio();
        qDebug() << "Frame Rate Range: [" << viewfinderSettings.minimumFrameRate()
                 << ", " << viewfinderSettings.maximumFrameRate() << "]";
        qDebug() << "Pixel Format: " << viewfinderSettings.pixelFormat();
        qDebug() << "-----------------------------------------";
    }

    camera->setViewfinderSettings(viewfinderSettings);
}

void RecordVideo::switchCamera(int index)
{
    currentCameraInfo = cameraComboBox->itemData(index).value<QCameraInfo>();
    camera->stop();
    camera = new QCamera(currentCameraInfo);
    camera->setViewfinder(viewfinder);
    camera->start();
    updateCameraSettings();
    updateViewfinderSettings();
}

void RecordVideo::updateScheduleTime()
{
    QTime time = QTime::fromString(schedule_time->text(), "HH:mm:ss"); // get what is currently on display
    time = time.addSecs(1);
    QString updatedTime = time.toString("HH:mm:ss");
    schedule_time->setText(updatedTime); // add one second to it
}
