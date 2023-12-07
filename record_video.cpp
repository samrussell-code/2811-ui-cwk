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

RecordVideo::RecordVideo(QWidget *parent) : QWidget(parent) {
    setupCamera();

    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *topLayout = new QHBoxLayout(centralWidget);
    leftLayout = new QVBoxLayout();
    rightLayout = new QVBoxLayout();
    eventInfoLayout = new QVBoxLayout();
    this->setAutoFillBackground(true); // background fill

    // create the generator that will set the initial label time.
    RandomTimeGenerator *gen = new RandomTimeGenerator;
    gen->scheduleTime();
    schedule_time_timer = new QLabel(gen->getSavedTimeDifference());
    schedule_time_timer->setMaximumHeight(20);
    eventInfoLayout->setAlignment(Qt::AlignTop);
    eventInfoLayout->addWidget(schedule_time_timer);

    schedule_time_daily = new QLabel(gen->getSavedTime());
    eventInfoLayout->addWidget(schedule_time_daily);

    leftLayout->addLayout(eventInfoLayout);
    leftLayout->addWidget(viewfinder);

    // create and start a timer to update the schedule time every second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &RecordVideo::updateScheduleTime);
    timer->start(1000);

    QString buildPath = QApplication::applicationDirPath();
    QString recordIconFilename = "record.png";
    QString recordingIconFilename = "recording.png";
    QString flipIconFilename = "flip_dark.png";
    QString crossIconFilename = "cross.png";
    QString confirmIconFilename = "tick.png";
    QString toggleModeIconFilename = "rotate.png";
    QString toggleModeIconPath = QDir(buildPath).filePath("icons/" + toggleModeIconFilename);
    QString confirmIconPath = QDir(buildPath).filePath("icons/" + confirmIconFilename);
    recordIconPath = QDir(buildPath).filePath("icons/" + recordIconFilename);
    recordingIconPath = QDir(buildPath).filePath("icons/" + recordingIconFilename);
    qDebug() << recordingIconPath << Qt::endl;
    crossIconPath = QDir(buildPath).filePath("icons/" + crossIconFilename);
    QString flipIconPath = QDir(buildPath).filePath("icons/" + flipIconFilename);
    qDebug() << recordIconPath;
    recordButton = new QPushButton(QIcon(recordIconPath), "");
    flipButton = new QPushButton(QIcon(flipIconPath), "");

    cameraComboBox = new QComboBox();

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "Name: " << cameraInfo.description();
        cameraComboBox->addItem(cameraInfo.description(), QVariant::fromValue(cameraInfo));
    }

    //setup some labels for buttons:
    record_label = new QLabel("Nothing recorded");
    toggled_label = new QLabel("Horizontal Video");

    connect(cameraComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            [=](int index) { switchCamera(index); });

    QString buttonStyle = "QPushButton{border:none;background-color:rgba(255, 255, 255,100);}";
    recordButton->setStyleSheet(buttonStyle); // Style sheet
    recordButton->setIconSize(QSize(75, 75));
    recordButton->setMinimumSize(75, 75);
    recordButton->setMaximumSize(75, 75);

    confirmButton = new QPushButton(QIcon(confirmIconPath), "");
    confirmButton->setStyleSheet(buttonStyle); // Style sheet
    confirmButton->setIconSize(QSize(75, 75));
    confirmButton->setMinimumSize(75, 75);
    confirmButton->setMaximumSize(75, 75);
    confirmButton->hide();  // Initially hide the confirm button

    flipButton->setStyleSheet(buttonStyle); // Style sheet
    flipButton->setIconSize(QSize(75, 75));
    flipButton->setMinimumSize(75, 75);
    flipButton->setMaximumSize(75, 75);

    toggleModeButton = new QPushButton(QIcon(toggleModeIconPath), "");
    toggleModeButton->setStyleSheet(buttonStyle); // Style sheet
    toggleModeButton->setIconSize(QSize(75, 75));
    toggleModeButton->setMinimumSize(75, 75);
    toggleModeButton->setMaximumSize(75, 75);
    connect(toggleModeButton, &QPushButton::clicked, this, &RecordVideo::toggleRecordingMode);

    connect(recordButton, &QPushButton::clicked, this, &RecordVideo::toggleIsRecording);

    connect(flipButton, &QPushButton::clicked, this, &RecordVideo::toggleFlipped);

    connect(confirmButton, &QPushButton::clicked, this, &RecordVideo::confirmVideoUpload);

    rightLayout->addWidget(cameraComboBox);

    // Create a grid layout for labels
    labelsGrid = new QGridLayout();
    flipped_label = new QLabel(cameraComboBox->currentText());
    addLabelToGrid(record_label, 0, 0);
    addLabelToGrid(flipped_label, 1, 0);
    addLabelToGrid(toggled_label, 2, 0);
    labelsGrid->setVerticalSpacing(10);
    rightLayout->setSpacing(175);
    rightLayout->addLayout(labelsGrid);


    // Create a horizontal layout for the record button and confirm button
    recordConfirmLayout = new QHBoxLayout();
    recordConfirmLayout->addWidget(recordButton);
    recordConfirmLayout->setSpacing(10);
    recordConfirmLayout->addWidget(confirmButton);

    QHBoxLayout *flipLayout;
    flipLayout = new QHBoxLayout();
    flipLayout->addWidget(flipButton);
    rightLayout->addLayout(flipLayout);
    rightLayout->addLayout(recordConfirmLayout);

    QHBoxLayout *rotateLayout;
    rotateLayout = new QHBoxLayout();
    rotateLayout->addWidget(toggleModeButton);
    rightLayout->addLayout(rotateLayout);

    topLayout->addLayout(leftLayout);
    topLayout->addLayout(rightLayout);

    setLayout(topLayout);

    setMinimumSize(774, 1040);
}

RecordVideo::~RecordVideo() {
    camera->stop();
}

void RecordVideo::addLabelToGrid(QLabel *label, int row, int column) {
    label->setAlignment(Qt::AlignCenter);
    label->setFrameShape(QFrame::Box);
    label->setLineWidth(2);
    label->setStyleSheet("QLabel { background-color: darkgrey; }");
    labelsGrid->addWidget(label, row, column);
}

void RecordVideo::setupCamera() {
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

void RecordVideo::toggleIsRecording() {
    if(record_label->text().contains("Nothing recorded")){
        record_label->setText("Recording");
        // Set the icon to "recording.png"
        qDebug() <<"Icon in loop: " << recordingIconPath << Qt::endl;
        recordButton->setIcon(QIcon(recordingIconPath));
    }
    else if(record_label->text().contains("Recording")){
        record_label->setText("Recorded");
        // Set the icon to "cross.png"
        recordButton->setIcon(QIcon(crossIconPath));
        confirmButton->show();
    }
    else{
        record_label->setText("Nothing recorded");
        recordButton->setIcon(QIcon(recordIconPath));
        confirmButton->hide();
    }
}
void RecordVideo::toggleFlipped() {
    // Find the index of the current camera in the list
    int currentIndex = cameraComboBox->currentIndex();

    // Calculate the index of the next camera, considering the list loop
    int nextIndex = (currentIndex + 1) % cameraComboBox->count();

    // Set the next camera as the current camera
    cameraComboBox->setCurrentIndex(nextIndex);
    switchCamera(nextIndex);

    // Update the flipped_label with the name of the current camera
    flipped_label->setText(cameraComboBox->currentText());
}
void RecordVideo::toggleRecordingMode() {
    verticalMode = !verticalMode;

    updateCameraSettings();
    updateViewfinderSettings();

    if(verticalMode == true){
        toggled_label->setText("Vertical Video");
    }
    else if (verticalMode == false){
        toggled_label->setText("Horizontal Video");
    }

    camera->stop();
    camera->start();
}

void RecordVideo::confirmVideoUpload(){
    recordButton->hide();
    confirmButton->hide();
    flipButton->hide();
    toggleModeButton->hide();
    cameraComboBox->hide();

    record_label->setText("Video uploaded!");
}

void RecordVideo::updateCameraSettings() {
    QVideoEncoderSettings videoSettings = recorder->videoSettings();

    if (verticalMode) {
        videoSettings.setResolution(720, 1280);
    }
    else {
        videoSettings.setResolution(1280, 720);
    }

    recorder->setVideoSettings(videoSettings);
}

void RecordVideo::updateViewfinderSettings() {
    QCameraViewfinderSettings viewfinderSettings = camera->viewfinderSettings();
    QList<QSize> availableSettings = camera->supportedViewfinderResolutions();

    qDebug() << "Supported Viewfinder Resolutions:";
    for (const QSize &resolution : availableSettings) {
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

void RecordVideo::switchCamera(int index) {
    currentCameraInfo = cameraComboBox->itemData(index).value<QCameraInfo>();
    camera->stop();
    camera = new QCamera(currentCameraInfo);
    camera->setViewfinder(viewfinder);
    camera->start();
    updateCameraSettings();
    updateViewfinderSettings();
}

void RecordVideo::updateScheduleTime() {
    QTime time = QTime::fromString(schedule_time_timer->text(), "HH:mm:ss"); // get what is currently on display
    time = time.addSecs(1);
    QString updatedTime = time.toString("HH:mm:ss");
    schedule_time_timer->setText(updatedTime); // add one second to it
}
