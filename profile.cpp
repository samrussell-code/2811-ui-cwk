#include "profile.h"
#include "friends.h"
#include <QtWidgets>
#include "user.h"
#include <QMainWindow>
#include <QDebug>

// The top part of the user profile
QWidget* Profile::profileInfo(int user_id) {
    QWidget* profile = new QWidget();
    QHBoxLayout* profilel = new QHBoxLayout();
    QVBoxLayout* info = new QVBoxLayout();
    QVBoxLayout* all = new QVBoxLayout();
    QWidget* allw = new QWidget();
    QWidget* infow = new QWidget();
    QLabel* name = new QLabel(usersArray[user_id].getName());
    QLabel* username = new QLabel(usersArray[user_id].getUsername());
    QLabel* description = new QLabel(usersArray[user_id].getDescription());
    QLabel* pfp = new QLabel();
    pfp->setStyleSheet("background-color: lightgreen; padding: 10px; border-radius: 25px;");
    pfp->setFixedSize(150, 150);
    info->addWidget(name);
    info->addWidget(username);
    info->addWidget(description);
    infow->setLayout(info);
    profilel->addWidget(pfp);
    profilel->addWidget(infow);
    profile->setLayout(profilel);
    QFrame* line = new QFrame;
    line->setFrameShape(QFrame::HLine);
    profile->setFixedSize(750, 200);
    all->addWidget(profile);
    all->addWidget(line);
    allw->setLayout(all);
    return allw;
}

// For displaying friends (each user)
QWidget* Profile::userWidget(int user_id) {
    QHBoxLayout* friendLayout = new QHBoxLayout();
    QLabel* pfp = new QLabel();
    pfp->setStyleSheet("background-color: lightgreen; padding: 10px; border-radius: 25px;");
    pfp->setFixedSize(50, 50);
    QHBoxLayout* namesL = new QHBoxLayout();
    QLabel* name = new QLabel(usersArray[user_id].getName());
    QLabel* username = new QLabel("@" + usersArray[user_id].getName());
    namesL->addWidget(name);
    namesL->addWidget(username);
    QWidget* namesW = new QWidget();
    namesW->setLayout(namesL);
    friendLayout->addWidget(pfp);
    friendLayout->addWidget(namesW);
    QWidget* friendsWidget = new QWidget();
    friendsWidget->setLayout(friendLayout);
    return friendsWidget;
}

// Add friend button functionality
void Profile::addFriend(int userFriendID, QPushButton* addButton) {
    usersArray[0].addFriend(userFriendID);
    addButton->setText("Added");
    //qDebug() << "friends" << usersArray[0].getFriends();
}

// Create all friends widget
QWidget* Profile::allFriends(int user_id) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();
    QList<int> friendslist = usersArray[user_id].getFriends();
    // Iterate for each user to check if they are in the friend list
    if (friendslist.isEmpty()) {
        QLabel* noFriendsLabel = new QLabel("No friends yet.");
        userl->addWidget(noFriendsLabel);
    }
    else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < friendslist.size(); j++) {
                if (i == friendslist[j]) {
                    QWidget* user = userWidget(i-1);
                    QPushButton* add = new QPushButton("Add");
                    connect(add, &QPushButton::clicked, [=]() {
                        addFriend(friendslist[j], add);
                    });

                    QHBoxLayout* userh = new QHBoxLayout();
                    userh->addWidget(user);
                    userh->addWidget(add);

                    QWidget* userhw = new QWidget();
                    userhw->setLayout(userh);

                    userl->addWidget(userhw);

                    QFrame* line = new QFrame;
                    line->setFrameShape(QFrame::HLine);
                    userl->addWidget(line);
                }
            }
        }
    }
    all->setLayout(userl);
    return all;
}

Profile::Profile(QWidget* parent, int user_id)
    : QWidget(parent) {

    usersArray = Friends::users;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("User Profile");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 25px; padding: 10px; text-align: center;");
    mainLayout->addWidget(titleLabel);

    // Profile Scroll Area
    QScrollArea* profileScrollArea = new QScrollArea(this);
    profileScrollArea->setWidgetResizable(true);

    // Create Profile Widget
    QWidget* profileWidget = new QWidget(this);
    QVBoxLayout* profileLayout = new QVBoxLayout(profileWidget);

    QWidget* profile_info = profileInfo(user_id);
    profileLayout->addWidget(profile_info);

    // Add the Friends and Videos buttons to the profile layout
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* videosButton = new QPushButton("Videos");
    QPushButton* friendsButton = new QPushButton("Friends");
    videosButton->setStyleSheet("background-color: black; color: white;");
    friendsButton->setStyleSheet("background-color: black; color: white;");
    buttons->addWidget(videosButton);
    buttons->addWidget(friendsButton);
    profileLayout->addLayout(buttons);

    // Friend Section Widget
    QWidget* friendWidget = allFriends(user_id);
    friendWidget->setVisible(false);
    profileLayout->addWidget(friendWidget);

    // Video Section Widget
    QWidget* videosWidget = new QWidget(this);
    QVBoxLayout* videosLayout = new QVBoxLayout(videosWidget);

    // Create three rectangles representing videos
    for (int i = 0; i < 3; ++i) {
        QFrame* videoRectangle = new QFrame();
        videoRectangle->setFrameShape(QFrame::Box);
        videoRectangle->setFrameShadow(QFrame::Raised);
        videoRectangle->setStyleSheet("background-color: grey;");

        QLabel* videoText = new QLabel("Video " + QString::number(i + 1));
        videoText->setAlignment(Qt::AlignCenter);
        QVBoxLayout* videoLayout = new QVBoxLayout(videoRectangle);
        QFont font = videoText->font();
        font.setPointSize(25);
        videoText->setFont(font);
        videoLayout->addWidget(videoText);

        videoRectangle->setFixedWidth(700);
        videoRectangle->setFixedHeight(500);

        videosLayout->addWidget(videoRectangle);
    }

    profileLayout->addWidget(videosWidget);
    videosWidget->setVisible(true);
    profileLayout->addWidget(videosWidget);

    profileScrollArea->setWidget(profileWidget);
    mainLayout->addWidget(profileScrollArea);

    setLayout(mainLayout);

    // Connect buttons to switch between sections
    QObject::connect(friendsButton, &QPushButton::clicked, [=]() {
        friendWidget->setVisible(true);
        videosWidget->setVisible(false);
        friendsButton->setStyleSheet("background-color: grey; color: white;");
        videosButton->setStyleSheet("background-color: black; color: white;");
    });

    QObject::connect(videosButton, &QPushButton::clicked, [=]() {
        friendWidget->setVisible(false);
        videosWidget->setVisible(true);
        friendsButton->setStyleSheet("background-color: black; color: white;");
        videosButton->setStyleSheet("background-color: grey; color: white;");
    });
}

