#include "profile.h"
#include "friends.h"
#include <QtWidgets>
#include "user.h"
#include <QMainWindow>
#include <QDebug>

//the top part of the user profile
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

//for displaying friends(each user)
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

//add friend button functionality
void Profile::addFriend(int userFriendID) {
    usersArray[0].addFriend(userFriendID);
   qDebug() << "friends" << usersArray[0].getFriends();
}

//create all friends widget
QWidget* Profile::allFriends(int user_id) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();
    QList<int> friendslist = usersArray[user_id].getFriends();
    for (int i = 0; i < 10; i++) {
       for (int j = 0; j < friendslist.size(); j++) {
          if (i == friendslist[j]) {
              QWidget* user = userWidget(i-1);
              QPushButton* add = new QPushButton("Add");
              connect(add, &QPushButton::clicked, [=]() {
                  addFriend(friendslist[j]);
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
        all->setLayout(userl);
        return all;
    }

//final friends Widget
QWidget* Profile::friendLayout(int user_id) {
  QVBoxLayout* friendl = new QVBoxLayout();
  QWidget* friendsw = new QWidget();
  QLabel* friendlabel = new QLabel("Friends");
  QWidget* friendlist = allFriends(user_id);
  friendl->addWidget(friendlabel);
  friendl->addWidget(friendlist);
  friendl->setAlignment(Qt::AlignTop);
  friendsw->setLayout(friendl);
  return friendsw;
}

//takes as argument user_id
Profile::Profile(QWidget* parent, int user_id)
    : QMainWindow(parent) {

    //pointer to users array
    usersArray = Friends::users;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    QLabel* titleLabel = new QLabel("User Profile");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 25px; padding: 10px; text-align: center;");
    mainLayout->addWidget(titleLabel);

    //top part of profile
    QWidget* profile_info = profileInfo(user_id);
    mainLayout->addWidget(profile_info);

    //videos layout(update that)
    QWidget* videosWidget = new QWidget(this);
    QVBoxLayout* x = new QVBoxLayout();
    QLabel* label = new QLabel("Videos");
    x->addWidget(label);
    videosWidget->setLayout(x);

    //friends layout
    QWidget* friendWidget = friendLayout(user_id);

    friendWidget->setVisible(false);
    videosWidget->setVisible(true);


    //buttons
    QHBoxLayout* buttons = new QHBoxLayout();
    QPushButton* videos = new QPushButton("Videos");
    QPushButton* friends = new QPushButton("Friends");
    videos->setStyleSheet("background-color: black; color: white;");
    friends->setStyleSheet("background-color: black; color: white;");
    buttons->addWidget(videos);
    buttons->addWidget(friends);
    QWidget* buttonsw = new QWidget();
    buttonsw->setLayout(buttons);
    mainLayout->addWidget(buttonsw);


    mainLayout->addWidget(friendWidget);
    mainLayout->addWidget(videosWidget);
    setStyleSheet("background-color: white;");

    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

   //friends button function
    QObject::connect(friends, &QPushButton::clicked, [=]() {
        friendWidget->setVisible(true);
        videosWidget->setVisible(false);
    });

    //videos button function
    QObject::connect(videos, &QPushButton::clicked, [=]() {
        friendWidget->setVisible(false);
        videosWidget->setVisible(true);
    });
}
