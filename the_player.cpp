//
//
//

#include "the_player.h"

// all buttons have been setup, store pointers here
void ThePlayer::setContent(std::vector<TheButtonInfo>* i) {
    infos = i;
    if (infos && !infos->empty()) {
        jumpTo(&infos->at(0)); // Start with the first video
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
}

void ThePlayer::nextVideo() {
    if (infos != nullptr && !infos->empty()) {
        // Increment the current video index and wrap around if needed
        updateCount = (updateCount + 1) % infos->size();
        jumpTo(&infos->at(updateCount));
    }
}

void ThePlayer::previousVideo() {
    if (infos != nullptr && !infos->empty()) {
        // Decrease the current video index and wrap around if needed
        updateCount = (updateCount - 1) % infos->size();
        jumpTo(&infos->at(updateCount));
    }
}
