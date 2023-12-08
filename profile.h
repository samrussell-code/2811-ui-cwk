#ifndef PROFILE_H
#define PROFILE_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "user.h"
#include "friends.h"
#include <QMainWindow>

class Profile : public QMainWindow
{
    Q_OBJECT

public:
    Profile(QWidget* parent = nullptr, int user_id = 0);
    QWidget* profileInfo(int user_id);
    QWidget* userWidget(int user_id);
    QWidget* allFriends(int user_id);
    QWidget* friendLayout(int user_id);
    void addFriend(int user_id);
private:

    User* usersArray;


};

#endif // PROFILE_H
