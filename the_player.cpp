//
//
//

#include "the_player.h"

ThePlayer::ThePlayer(RecordVideo *recordVideo, QObject *parent)
    : QMediaPlayer(parent),
    recordVideoInstance(recordVideo) {
    connect(recordVideoInstance, &RecordVideo::recordingConfirmedChanged, this, &ThePlayer::handleRecordingConfirmedChanged);
    setVolume(0); // be slightly less annoying
    connect (this, SIGNAL (stateChanged(QMediaPlayer::State)), this, SLOT (playStateChanged(QMediaPlayer::State)) );
}

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButtonInfo>* i) {
    infos = i;
    if (infos && !infos->empty()) {
        jumpTo(&infos->at(1)); // Start with the first video
    }
}

void ThePlayer::playStateChanged (QMediaPlayer::State ms) {
    switch (ms) {
    case QMediaPlayer::State::StoppedState:
        play(); // starting playing again...
        break;
    default:
        break;
    }
}

void ThePlayer::jumpTo (TheButtonInfo* button) {
    setMedia( * button -> url);
    play();
    resetLikeState();
}

void ThePlayer::nextVideo() {
    if (infos != nullptr && !infos->empty()) {
        // Increment the current video index and wrap around if needed
        updateCount = (updateCount + 1) % infos->size();

        // If the current video is the 0th one, skip it
        if (updateCount == 0) {
            updateCount = (updateCount + 1) % infos->size();
        }

        jumpTo(&infos->at(updateCount));
    }
}


void ThePlayer::previousVideo() {
    if (infos != nullptr && !infos->empty()) {
        // Decrease the current video index and wrap around if needed
        updateCount = (updateCount - 1 + infos->size()) % infos->size();

        // If the current video is the 0th one, skip it
        if (updateCount == 0) {
            updateCount = (updateCount - 1 + infos->size()) % infos->size();
        }

        jumpTo(&infos->at(updateCount));
    }
}

void ThePlayer::handleRecordingConfirmedChanged(bool confirmed) {
    qDebug()<< "reached event in the player" << Qt::endl;
    // Update your logic to handle the recording confirmation change
    // For example, stop skipping the first video when recording is confirmed
    if (confirmed) {
        // Check if the current video is the 0th one and take appropriate action
        jumpTo(&infos->at(0));
        qDebug()<< "Showing the users video" << Qt::endl;
        // Do something to handle not skipping the first video
    }
}
