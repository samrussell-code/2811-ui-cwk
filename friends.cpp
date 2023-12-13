#include "friends.h"
#include <QtWidgets>
#include "user.h"
#include "profile.h"
#include <QMainWindow>
#include <QDebug>

//updates the window
void Friends::updateUI(int action) {

    QWidget* friendsList = friendLayout(users[0], users);
    friendsScrollArea->setWidget(friendsList);

    QWidget* recommendedList = recommendedLayout(users[0], users);
    recommendedScrollArea->setWidget(recommendedList);

    QWidget* requestsList = requestsLayout(users[0], users);
    requestsScrollArea->setWidget(requestsList);

    // This is for the category we are in
    if (action==1){
        friendsLayout->setCurrentIndex(0);
    }
    if (action==2){
        friendsLayout->setCurrentIndex(1);
    }
    if (action==3){
        friendsLayout->setCurrentIndex(2);
    }

}

// Adds Friend(methos from user class)
void Friends::addFriend(int userFriendID) {
    users[0].addFriend(userFriendID);
    //qDebug() << "friends" << users[0].getFriends();
}

// Accept a friend request
void Friends::acceptUser(int userID) {
    //add to friends list
    addFriend(userID);
    QList<int> requests = users[0].getRequests();
    //remove from requests list
    requests.removeOne(userID);
    users[0].setRequests(requests);
    //qDebug() << "friends" << users[0].getFriends();
    //qDebug() << "requests" << users[0].getRequests();
    updateUI(3);
}

//declines a friend request
void Friends::declineUser(int userID) {
    QList<int> requests =users[0].getRequests();
    //removes the user from request list
    requests.removeOne(userID);
    users[0].setRequests(requests);

    updateUI(3);
}

//add a friend from recommended
void Friends::addRecommended(int userID) {
    //qDebug() << "Add button clicked for user with ID:" << userID;
    //adds in friend list
    addFriend(userID);
    QList<int> recommended = users[0].getRecommended();
    //remove from friend list
    recommended.removeOne(userID);
    users[0].setRecommended(recommended);

    updateUI(2);

}

//creates a widget for individual user
QWidget* Friends::userWidget(User& user) {
    QHBoxLayout* friendLayout = new QHBoxLayout();
    QLabel* pfp = new QLabel();
    pfp->setStyleSheet("background-color: lightgreen; padding: 10px; border-radius: 25px;");
    pfp->setFixedSize(50, 50);
    QLabel* nameLabel = new QLabel("<b>" + user.getName() + "</b>\n@" + user.getUsername());
    friendLayout->addWidget(pfp);
    friendLayout->addWidget(nameLabel);
    QWidget* friendsWidget = new QWidget();
    friendsWidget->setLayout(friendLayout);
    return friendsWidget;
}

// Creates a widget with all individual users(that are in friends) widgets
QWidget* Friends::allUsersViewProfile(User* users, User& user) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();
    QList<int> friendslist = user.getFriends();
    //loops for each user to check if they are in the list
    for (int i = 0; i < 10; i++ ) {
        for (int j = 0; j < friendslist.size(); j++) {
            if (i == friendslist[j]) {
                QWidget* user = userWidget(users[i-1]);
                QPushButton* view = new QPushButton("View Profile");
                view->setStyleSheet("background-color: black; color: white;");
                connect(view, &QPushButton::clicked, [=]() {
                    QWidget* profileWidget = new Profile(this,i-1);
                    friendsLayout->addWidget(profileWidget);
                    friendsLayout->setCurrentWidget(profileWidget);
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


// Creates a widget with all individual users(that are in recommended) widgets
QWidget* Friends::allUsersAdd(User* users, User& user) {
    QWidget* all = new QWidget();
    QVBoxLayout* userl = new QVBoxLayout();
    QList<int> friendslist = user.getRecommended();
    //loops for each user to check if they are in the list
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

// Creates a widget with all individual users(that are in requests) widgets
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

// Creates the final friend layout
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

// Creates the final recommended layout
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

// Creates the final requests layout
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

// Creates instances of users for examples
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
    : QWidget(parent) {

    // Set user that uses the app
    User current_user = Friends::users[0];

    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Title
    QLabel* titleLabel = new QLabel("Friends");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 24px; padding: 10px; text-align: center;");
    mainLayout->addWidget(titleLabel);
    setStyleSheet("background-color: white;");

    // Search Bar
    QHBoxLayout* search = new QHBoxLayout(this);
    QLineEdit* searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Search for users...");
    searchBar->setFixedHeight(40);
    search->addWidget(searchBar);
    QPushButton* searchButton = new QPushButton("Search");
    searchButton->setFixedHeight(40);
    search->addWidget(searchButton);
    mainLayout->addLayout(search);
    connect(searchButton, &QPushButton::clicked, [=](){
        QString searchText = searchBar->text();
        QMessageBox::information(this, "Search Result", "You made a search for: " + searchText);
    });

    // Friends Content (Friends, Recommended, Requests)
    friendsLayout = new QStackedLayout(this);

    // Scroll area for friends
    friendsScrollArea = new QScrollArea(this);
    QWidget* friendsListWidget = friendLayout(current_user, users);
    friendsScrollArea->setWidgetResizable(true);
    friendsScrollArea->setWidget(friendsListWidget);
    friendsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    friendsLayout->addWidget(friendsScrollArea);

    // Scroll area for recommended
    recommendedScrollArea = new QScrollArea(this);
    QWidget* recommendedListWidget = recommendedLayout(current_user, users);
    recommendedScrollArea->setWidgetResizable(true);
    recommendedScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    recommendedScrollArea->setWidget(recommendedListWidget);
    friendsLayout->addWidget(recommendedScrollArea);

    // Scroll area for requests
    requestsScrollArea = new QScrollArea(this);
    QWidget* requestsListWidget = requestsLayout(current_user, users);
    requestsScrollArea->setWidgetResizable(true);
    requestsScrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    requestsScrollArea->setWidget(requestsListWidget);
    friendsLayout->addWidget(requestsScrollArea);

    // Top buttons to change the page contents
    QPushButton* friendsButton = new QPushButton("Friends", this);
    QPushButton* recommendedButton = new QPushButton("Recommended", this);
    QPushButton* requestsButton = new QPushButton("Requests", this);
    friendsButton->setStyleSheet("background-color: grey; color: white;");
    recommendedButton->setStyleSheet("background-color: black; color: white;");
    requestsButton->setStyleSheet("background-color: black; color: white;");

    connect(friendsButton, &QPushButton::clicked, [=]() {
        updateUI(1);
        friendsLayout->setCurrentIndex(0);
        friendsButton->setStyleSheet("background-color: grey; color: white;");
        recommendedButton->setStyleSheet("background-color: black; color: white;");
        requestsButton->setStyleSheet("background-color: black; color: white;");
    });

    connect(recommendedButton, &QPushButton::clicked, [=]() {
        updateUI(2);
        friendsLayout->setCurrentIndex(1);
        friendsButton->setStyleSheet("background-color: black; color: white;");
        recommendedButton->setStyleSheet("background-color: grey; color: white;");
        requestsButton->setStyleSheet("background-color: black; color: white;");
    });

    connect(requestsButton, &QPushButton::clicked, [=]() {
        updateUI(3);
        friendsLayout->setCurrentIndex(2);
        friendsButton->setStyleSheet("background-color: black; color: white;");
        recommendedButton->setStyleSheet("background-color: black; color: white;");
        requestsButton->setStyleSheet("background-color: grey; color: white;");
    });

    QHBoxLayout* buttonsRowLayout = new QHBoxLayout();
    buttonsRowLayout->addWidget(friendsButton);
    buttonsRowLayout->addWidget(recommendedButton);
    buttonsRowLayout->addWidget(requestsButton);
    mainLayout->addLayout(buttonsRowLayout);

    mainLayout->addLayout(friendsLayout);

}
