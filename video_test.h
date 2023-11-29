#ifndef VIDEO_TEST_H
#define VIDEO_TEST_H

#include <QMainWindow>
#include <QCamera>
#include <QCameraViewfinder>
#include <QVBoxLayout>

class VideoTest : public QMainWindow
{
    Q_OBJECT

public:
    explicit VideoTest(QWidget *parent = nullptr);
    ~VideoTest();

private:
    QCamera *camera;
    QCameraViewfinder *viewfinder;
};

#endif // VIDEO_TEST_H
