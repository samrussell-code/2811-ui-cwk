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

private:
    void setupCamera();

    QCamera *camera;
    QCameraViewfinder *viewfinder;
    QMediaRecorder *recorder;

    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QVBoxLayout *eventInfoLayout;

    QLabel *label;
    QPushButton *searchButton;
    QPushButton *closeButton;
};

#endif // MAINWINDOW_H
