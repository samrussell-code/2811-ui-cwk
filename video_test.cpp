#include "video_test.h"

#include <QtMultimedia/QCameraInfo>

VideoTest::VideoTest(QWidget *parent) :
    QMainWindow(parent),
    camera(new QCamera(this)),
    viewfinder(new QCameraViewfinder(this))
{
    camera->setViewfinder(viewfinder);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(viewfinder);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);

    setCentralWidget(centralWidget);

    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras) {
        qDebug() << "Name: " << cameraInfo.deviceName();
        qDebug() << "Position: " << cameraInfo.position();
        qDebug() << "Orientation: " << cameraInfo.orientation();
    }

    // Start the camera
    camera->start();
    if (camera->status() == QCamera::Status::ActiveStatus) {
        qDebug() << "Camera started successfully";
    } else {
        qDebug() << "Error starting camera:" << camera->errorString();
    }

}

VideoTest::~VideoTest()
{
    // Stop the camera when the window is closed
    camera->stop();
}
