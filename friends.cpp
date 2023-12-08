#include "friends.h"
#include <QtWidgets>
#include "user.h"
#include "profile.h"
#include <QMainWindow>
#include <QDebug>

//updates the window
void Friends::updateUI() {
    Friends *friendsWindow = new Friends(this);
    this->close();
    friendsWindow->show();
 }

//adds Friend
void Friends::addFriend(int userFriendID) {
    users[0].addFriend(userFriendID);
    //qDebug() << "friends" << users[0].getFriends();
}

//accept a friend request
void Friends::acceptUser(int userID) {
    //add to friends list
    addFriend(userID);
    QList<int> requests = users[0].getRequests();
    //remove from requests list
    requests.removeOne(userID);
    users[0].setRequests(requests);
    qDebug() << "friends" << users[0].getFriends();
    qDebug() << "requests" << users[0].getRequests();
    updateUI();
}

//declines a friend request
void Friends::declineUser(int userID) {
    QList<int> requests =users[0].getRequests();
    requests.removeOne(userID);
    users[0].setRequests(requests);
    updateUI();
}

//add a friend from recommended
void Friends::addRecommended(int userID) {
    qDebug() << "Add button clicked for user with ID:" << userID;
    //adds in friend list
    addFriend(userID);
    QList<int> recommended = users[0].getRecommended();
    //remove from friend list
    recommended.removeOne(userID);
    users[0].setRecommended(recommended);
    updateUI();

}

//creates a widget for user
QWidget* Friends::userWidget(User& user) {
    QHBoxLayout* friendLayout = new QHBoxLayout();
    QLabel* pfp = new QLabel();
    pfp->setStyleSheet("background-color: lightgreen; padding: 10px; border-radius: 25px;");
    pfp->setFixedSize(50, 50);
    QHBoxLayout* namesL = new QHBoxLayout();
    QLabel* name = new QLabel(user.getName());
    QLabel* username = new QLabel("@" + user.getName());
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

//creates many users that need view profile button(friends)
QWidget* Friends::allUsersViewProfile(User* users, User& user) {
        QWidget* all = new QWidget();
        QVBoxLayout* userl = new QVBoxLayout();
        QList<int> friendslist = user.getFriends();
        for (int i = 0; i < 10; i++ ) {
            for (int j = 0; j < friendslist.size(); j++) {
                if (i == friendslist[j]) {
                    QWidget* user = userWidget(users[i-1]);
                    QPushButton* view = new QPushButton("View Profile");
                    view->setStyleSheet("background-color: black; color: white;");
                    connect(view, &QPushButton::clicked, [=]() {
                           // Create and show the profile window
                           Profile *profileWindow = new Profile(this, i-1);
                           this->close();
                           profileWindow->show();
                       });
                    QHBoxLayout* userh = new QHBoxLayout();
                    userh->addWidget(user);
                    userh->addWidget(view);

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


//creates many users that need add button(recommended)
QWidget* Friends::allUsersAdd(User* users, User& user) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();
    QList<int> friendslist = user.getRecommended();
    //loops for each user
    for (int i = 0; i < 10; i++) {
       //loops for each user in the friend list
       for (int j = 0; j < friendslist.size(); j++) {
          if (i == friendslist[j]) {

              QWidget* userW = userWidget(users[i-1]);

              //add button
              QPushButton* add = new QPushButton("Add");
              add->setStyleSheet("background-color: black; color: white;");
              connect(add, &QPushButton::clicked, [=]() {
                  addRecommended(friendslist[j]);
              });

              QHBoxLayout* userh = new QHBoxLayout();
              userh->addWidget(userW);
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

//creates many users that need accept-decline button(requests)
QWidget* Friends::allUsersAccept(User* users, User& user) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();

    QList<int> requestslist = user.getRequests();

    //loops for each user
    for (int i = 0; i < 10; i++) {
        //loops for each user in request list
        for (int j = 0; j < requestslist.size(); j++) {
            if (i == requestslist[j]) {
                QWidget* userwidget = userWidget(users[i-1]);

                //buttons
                QPushButton* accept = new QPushButton("Accept");
                QPushButton* decline = new QPushButton("Decline");

                accept->setStyleSheet("background-color: black; color: white;");
                decline->setStyleSheet("background-color: lightgrey; color: black;");

                connect(accept, &QPushButton::clicked, [=]() {
                   acceptUser(requestslist[j]);
                });

                connect(decline, &QPushButton::clicked, [=]() {
                   declineUser(requestslist[j]);
                });


                QHBoxLayout* buttonLayout = new QHBoxLayout();
                buttonLayout->addWidget(accept);
                buttonLayout->addWidget(decline);

                QVBoxLayout* userLayout = new QVBoxLayout();
                userLayout->addWidget(userwidget);
                userLayout->addLayout(buttonLayout);

                QWidget* userContainer = new QWidget();
                userContainer->setLayout(userLayout);

                userl->addWidget(userContainer);

                QFrame* line = new QFrame;
                line->setFrameShape(QFrame::HLine);
                userl->addWidget(line);
            }
        }
    }

    all->setLayout(userl);
    return all;
}

//creates the final friend layout
QWidget* Friends::friendLayout(User& user, User* users) {
  QVBoxLayout* friendl = new QVBoxLayout();
  QWidget* friendsw = new QWidget();
  QLabel* label = new QLabel("Friends");
  label->setStyleSheet("font-size: 16pt; font-weight: bold;");
  QWidget* friendlist = allUsersViewProfile(users, user);
  friendl->addWidget(label);
  friendl->addWidget(friendlist);
  friendl->setAlignment(Qt::AlignTop);
  friendsw->setLayout(friendl);
  return friendsw;
}

//creates the final recommended layout
QWidget* Friends::recommendedLayout(User& user, User* users) {
  QVBoxLayout* friendl = new QVBoxLayout();
  QWidget* friendsw = new QWidget();
  QLabel* label = new QLabel("Recommended");
  label->setStyleSheet("font-size: 16pt; font-weight: bold;");
  QWidget* friendlist = allUsersAdd(users, user);
  friendl->addWidget(label);
  friendl->addWidget(friendlist);
  friendl->setAlignment(Qt::AlignTop);
  friendsw->setLayout(friendl);
  return friendsw;
}

//creates the final requests layout
QWidget* Friends::requestsLayout(User& user, User* users) {
  QVBoxLayout* friendl = new QVBoxLayout();
  QWidget* friendsw = new QWidget();
  QLabel* label = new QLabel("Requests");
  label->setStyleSheet("font-size: 16pt; font-weight: bold;");
  QWidget* friendlist = allUsersAccept(users, user);
  friendl->addWidget(label);
  friendl->addWidget(friendlist);
  friendl->setAlignment(Qt::AlignTop);
  friendsw->setLayout(friendl);
  return friendsw;
}

//creates instances of users for examples
QList<int> initialFriends = {2,3};
QList<int> initialFriends2 = {7,4,6};
QList<int> emptyFriends = {};
QList<int> recommended = {9, 4, 5, 6};
QList<int> requests = {7,8,9};

User Friends::users[10] = {User("name1", "username1", "users description", 0, initialFriends, recommended, requests),
                  User("name2", "username2", "user description 2", 1, initialFriends2, emptyFriends, emptyFriends),
                  User("name3", "username3", "user description 3", 2, emptyFriends, emptyFriends, emptyFriends),
                  User("name4", "username4", "user description 4", 3, emptyFriends, emptyFriends, emptyFriends),
                  User("name5", "username5", "user description 5", 4, emptyFriends, emptyFriends, emptyFriends),
                  User("name6", "username6", "user description 6", 5, emptyFriends, emptyFriends, emptyFriends),
                  User("name7", "username7", "user description 7", 6, emptyFriends, emptyFriends, emptyFriends),
                  User("name8", "username8", "user description 8", 7, emptyFriends, emptyFriends, emptyFriends),
                  User("name9", "username9", "user description 9", 8, emptyFriends, emptyFriends, emptyFriends),
                  User("name10", "username10", "user description 10", 9, emptyFriends,emptyFriends, emptyFriends )};


Friends::Friends(QWidget* parent)
    : QMainWindow(parent) {

    //Set user that uses the app
    User current_user = Friends::users[0];

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    //Title
    QLabel* titleLabel = new QLabel("Friends");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 24px; padding: 10px; text-align: center;");
    mainLayout->addWidget(titleLabel);
    setStyleSheet("background-color: white;");

    //Search Bar
    QHBoxLayout* search = new QHBoxLayout(this);
    QLineEdit* searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Search for users...");
    search->addWidget(searchBar);
    QPushButton* searchButton = new QPushButton("Search");
    search->addWidget(searchButton);
    mainLayout->addLayout(search);

    //Friends Content
    QHBoxLayout* content = new QHBoxLayout(this);
    QWidget* friendsList = new QWidget(this);
    QWidget* recommendedList = new QWidget(this);
    QWidget* requestsList = new QWidget(this);

    friendsList = friendLayout(current_user, users);
    recommendedList = recommendedLayout(current_user, users);
    requestsList = requestsLayout(current_user, users);

    content->addWidget(friendsList);
    content->addWidget(recommendedList);
    content->addWidget(requestsList);

    QWidget* contentw = new QWidget();
    contentw->setLayout(content);
    mainLayout->addWidget(contentw);

    //Window
    QWidget* centralWidget = new QWidget;
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);
}
