#ifndef RECORDVIDEO_H
#define RECORDVIDEO_H

#include "qcamerainfo.h"
#include <QMainWindow>
#include <QWidget>
#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QtMultimedia/QMediaRecorder>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>

Q_DECLARE_METATYPE(QCameraInfo)

class RecordVideo : public QWidget
{
    Q_OBJECT


public:
    explicit RecordVideo(QWidget *parent = nullptr);
    ~RecordVideo();

private:
    void setupCamera();
    void updateCameraSettings();
    void updateViewfinderSettings();

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QMediaRecorder *recorder;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *eventInfoLayout;

    QLabel *schedule_time;
    QPushButton *recordButton;
    QPushButton *flipButton;
    QPushButton *toggleModeButton;
    QComboBox *cameraComboBox;

    bool verticalMode;
    QCameraInfo currentCameraInfo;

private slots:
    void toggleRecordingMode();
    void switchCamera(int index);
};

#endif // RECORDVIDEO_H
