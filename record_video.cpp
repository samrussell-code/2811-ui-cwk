#include "record_video.h"
#include "qcamerainfo.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QCameraViewfinder>
#include <QMultimedia>
#include <QComboBox>
#include <QPalette>

RecordVideo::RecordVideo(QWidget *parent) : QWidget(parent)
{
    setupCamera();

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(centralWidget);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    eventInfoLayout = new QVBoxLayout();
    this->setAutoFillBackground(true); // background fill

    schedule_time = new QLabel("00:00:00");
    schedule_time->setMaximumHeight(20);
    eventInfoLayout->setAlignment(Qt::AlignTop);
    eventInfoLayout->addWidget(schedule_time);

    leftLayout->addLayout(eventInfoLayout);
    leftLayout->addWidget(viewfinder);

    recordButton = new QPushButton(QIcon("C:/Users/russe/OneDrive/Y2/S1/User_Interfaces/2811-ui-cwk/icons/record.png"), "");
    flipButton = new QPushButton(QIcon("C:/Users/russe/OneDrive/Y2/S1/User_Interfaces/2811-ui-cwk/icons/flip_dark.png"), "");

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

    //setWindowTitle("tomeo");
    // THIS ONLY WORKS BECAUSE IT IS THE SAME SIZE AS THE WINDOW ON STARTUP
    // IF YOU EXPAND THE WINDOW, OR INCREASE THIS SIZE, THE LAYOUT WILL
    // EXCEED THE BOUNDS OF THE WINDOW
    setMinimumSize(774,1040);
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
    videoSettings.setQuality(QMultimedia::VeryHighQuality);
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
    for (const QSize& resolution : availableSettings) {
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
