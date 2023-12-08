#ifndef FRIENDS_H
#define FRIENDS_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "user.h"
#include <QMainWindow>
class Profile;
class Friends : public QMainWindow
{
    Q_OBJECT

public:

    Friends(QWidget* parent = nullptr);
    void updateUI();
    QWidget* userWidget(User& user);
    QWidget* allUsersAdd(User* users, User& user);
    QWidget* allUsersViewProfile(User* users, User& user);
    QWidget* allUsersAccept(User* users, User& user);
    QWidget* friendLayout(User& user, User* users);
    QWidget* recommendedLayout(User& user, User* users);
    QWidget* requestsLayout(User& user, User* users);
    void addFriend(int userFriendID);
    void acceptUser(int userID);
    void declineUser(int userID);
    void addRecommended(int userID);
    void clearLayout(QLayout* layout);
    static User users[10];

private:


};

#endif // FRIENDS_H
