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
#include <QWidget>

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
    QGridLayout *labelsGrid;
    QLabel *schedule_time_timer;
    QLabel *schedule_time_daily;
    QLabel *record_label;
    QLabel *flipped_label;
    QLabel *toggled_label;
    QPushButton *recordButton;
    QPushButton *flipButton;
    QPushButton *toggleModeButton;
    QComboBox *cameraComboBox;

    bool verticalMode;
    QCameraInfo currentCameraInfo;

    QTimer *timer;  // Added QTimer member variable

private slots:
    void updateScheduleTime();
    void toggleRecordingMode();
    void toggleIsRecording();
    void toggleFlipped();
    void switchCamera(int index);
    void addLabelToGrid(QLabel *label, int row, int column);
};

#endif // RECORDVIDEO_H






