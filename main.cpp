#include "video_test.h"
#include "record_video.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RecordVideo w;
    w.show();
    return a.exec();
}
