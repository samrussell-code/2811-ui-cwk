//
//
//

#ifndef CW2_THE_PLAYER_H
#define CW2_THE_PLAYER_H


#include <QApplication>
#include <QMediaPlayer>
#include "record_video.h"
#include "the_button.h"
#include <vector>
#include <QTimer>

class ThePlayer : public QMediaPlayer {

    Q_OBJECT

private:
    std::vector<TheButtonInfo>* infos;
    std::vector<TheButton*>* buttons;
    QTimer* mTimer;
    long updateCount = 1; // start at 1 to ignore the users video
    RecordVideo *recordVideoInstance;

public:
    ThePlayer(RecordVideo *recordVideo, QObject *parent = nullptr);
    ThePlayer() : QMediaPlayer(NULL) {
        setVolume(0); // be slightly less annoying
        connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
    }

    // all buttons have been setup, store pointers here
    void setContent(std::vector<TheButtonInfo>* i);


private slots:

    // change the image and video for one button every one second

    void playStateChanged (QMediaPlayer::State ms);

public slots:

    // start playing this ButtonInfo
    void jumpTo (TheButtonInfo* button);
    void nextVideo();
    void previousVideo();
    void handleRecordingConfirmedChanged(bool confirmed);

};

#endif //CW2_THE_PLAYER_H
