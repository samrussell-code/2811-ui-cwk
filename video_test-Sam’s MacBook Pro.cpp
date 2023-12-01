#include "video_test.h"

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

    // Start the camera
    camera->start();
    qDebug() << "Camera state:" << camera->state();
    qDebug() << "Camera status:" << camera->status();
    if (camera->status() == QCamera::ActiveStatus) {
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
