#include "friends.h"
#include <QtWidgets>
#include "user.h"
#include "profile.h"

User::User():
    name(""), username(""), description(""), id(0), friends(QList<int>{}), recommended(QList<int>{}), requests(QList<int>{})
{
}

User::User(const QString& name, const QString& username, QString status, int id, QList<int> &friends, QList<int> &recommended, QList<int> &requests)
    : name(name), username(username), description(status), id(id), friends(friends), recommended(recommended), requests(requests)
{
}

QString User::getName() const {
    return name;
}

QString User::getUsername() const {
    return username;
}

QString User::getDescription() {
    return description;
}

QList<int> User::getFriends(){
    return friends;
}

QList<int> User::getRecommended(){
    return recommended;
}

QList<int> User::getRequests(){
    return requests;
}

void User::setRecommended(QList<int> recommendedl){
    recommended = recommendedl;
}

void User::setRequests(QList<int> requestsl){
    requests = requestsl;
}

int User::getId() const {
    return id;
}

void User::addFriend(int friendID) {
    if (!friends.contains(friendID)) {
        friends.append(friendID);
    }
}

void User::removeFriend(int friendId) {
    int index = friends.indexOf(friendId);
    if (index != -1) {
        friends.removeAt(index);
    }
}



