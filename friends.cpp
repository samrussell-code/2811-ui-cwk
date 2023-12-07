#include "friends.h"
#include <QtWidgets>

Friends::Friends(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    setStyleSheet("background-color: white;");

    QLineEdit* searchBar = new QLineEdit;
    searchBar->setPlaceholderText("Search for users...");

    QLabel* titleLabel = new QLabel("Friends");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 24px; padding: 10px; text-align: center;");

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(searchBar);

    QGroupBox* profilesGroup = new QGroupBox("Friends Profiles");
    QVBoxLayout* profilesLayout = new QVBoxLayout(profilesGroup);

    // Add round colored rectangles for users
    QHBoxLayout* friend1Layout = new QHBoxLayout;
    QLabel* friend1Label = new QLabel;
    friend1Label->setStyleSheet("background-color: lightblue; padding: 10px; border-radius: 25px;");
    friend1Label->setFixedSize(50, 50);

    QLabel* friend1NameLabel = new QLabel("User 1\n@user1");
    friend1NameLabel->setFixedSize(150, 50);

    QPushButton* friend1Button = new QPushButton("View Profile");
    friend1Button->setFixedSize(100, 30);
    friend1Button->setStyleSheet("background-color: black; color: white;");

    friend1Layout->addWidget(friend1Label);
    friend1Layout->addWidget(friend1NameLabel);
    friend1Layout->addWidget(friend1Button);

    QHBoxLayout* friend2Layout = new QHBoxLayout;
    QLabel* friend2Label = new QLabel;
    friend2Label->setStyleSheet("background-color: lightgreen; padding: 10px; border-radius: 25px;");
    friend2Label->setFixedSize(50, 50);

    QLabel* friend2NameLabel = new QLabel("User 3\n@user3");
    friend2NameLabel->setFixedSize(150, 50);

    QPushButton* friend2Button = new QPushButton("View Profile");
    friend2Button->setFixedSize(100, 30);
    friend2Button->setStyleSheet("background-color: black; color: white;");

    friend2Layout->addWidget(friend2Label);
    friend2Layout->addWidget(friend2NameLabel);
    friend2Layout->addWidget(friend2Button);

    profilesLayout->addLayout(friend1Layout);
    profilesLayout->addLayout(friend2Layout);

    QGroupBox* recommendedGroup = new QGroupBox("Recommended Friends");
    QVBoxLayout* recommendedLayout = new QVBoxLayout(recommendedGroup);

    // Add round colored rectangles for recommended friends
    QHBoxLayout* recommendedFriendLayout = new QHBoxLayout;
    QLabel* recommendedFriendLabel = new QLabel;
    recommendedFriendLabel->setStyleSheet("background-color: lightcoral; padding: 10px; border-radius: 25px;");
    recommendedFriendLabel->setFixedSize(50, 50);

    QLabel* recommendedFriendNameLabel = new QLabel("User 2\n@user2");
    recommendedFriendNameLabel->setFixedSize(150, 50);

    QPushButton* recommendedButton = new QPushButton("Add Friend");
    recommendedButton->setFixedSize(100, 30);
    recommendedButton->setStyleSheet("background-color: black; color: white;");

    recommendedFriendLayout->addWidget(recommendedFriendLabel);
    recommendedFriendLayout->addWidget(recommendedFriendNameLabel);
    recommendedFriendLayout->addWidget(recommendedButton);

    recommendedLayout->addLayout(recommendedFriendLayout);

    QGroupBox* requestsGroup = new QGroupBox("Friend Requests");
    QVBoxLayout* requestsLayout = new QVBoxLayout(requestsGroup);

    // Add round colored rectangles for friend requests
    QHBoxLayout* friendRequestLayout = new QHBoxLayout;
    QLabel* friendRequestLabel = new QLabel;
    friendRequestLabel->setStyleSheet("background-color: lightsalmon; padding: 10px; border-radius: 25px;");
    friendRequestLabel->setFixedSize(50, 50);

    QLabel* friendRequestNameLabel = new QLabel("User 4\n@user4");
    friendRequestNameLabel->setFixedSize(150, 50);

    QPushButton* requestAcceptButton = new QPushButton("Accept");
    requestAcceptButton->setFixedSize(75, 30);
    requestAcceptButton->setStyleSheet("background-color: black; color: white;");

    QPushButton* requestDeclineButton = new QPushButton("Decline");
    requestDeclineButton->setFixedSize(75, 30);
    requestDeclineButton->setStyleSheet("background-color: lightgrey; color: black;");

    friendRequestLayout->addWidget(friendRequestLabel);
    friendRequestLayout->addWidget(friendRequestNameLabel);
    friendRequestLayout->addWidget(requestAcceptButton);
    friendRequestLayout->addWidget(requestDeclineButton);

    requestsLayout->addLayout(friendRequestLayout);

    mainLayout->addWidget(profilesGroup);
    mainLayout->addWidget(recommendedGroup);
    mainLayout->addWidget(requestsGroup);
}

