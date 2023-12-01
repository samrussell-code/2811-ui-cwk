// record_video.h

#ifndef RECORDVIDEO_H
#define RECORDVIDEO_H

#include <QMainWindow>
#include <QtMultimedia/QCamera>
#include <QtMultimediaWidgets/QCameraViewfinder>
#include <QtMultimedia/QMediaRecorder>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class RecordVideo : public QMainWindow
{
    Q_OBJECT

public:
    explicit RecordVideo(QWidget *parent = nullptr);
    ~RecordVideo();

private slots:
    void toggleRecordingMode();
    void updateCameraSettings();
    void updateViewfinderSettings();

private:
    void setupCamera();

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QMediaRecorder *recorder;

    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *eventInfoLayout;

    QLabel *schedule_time;
    QPushButton *recordButton;
    QPushButton *flipButton;

    bool verticalMode;
};

#endif // RECORDVIDEO_H
