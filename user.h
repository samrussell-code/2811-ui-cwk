// user.h
#ifndef USER_H
#define USER_H

#include <QString>
#include <QList>

class User {
public:
    User(const QString& name, const QString& username, QString descriprion, int id, QList<int>& friends, QList<int> &recomended, QList<int> &requests);
    User();
    QString getName() const;
    QString getUsername() const;
    QString getDescription();
    QList<int> getFriends();
    QList<int> getRecommended();
    QList<int> getRequests();
    int getId() const;
    QList<int> setFriends(QList<int> friendsl);
    void setRecommended(QList<int> recommendedl);
    void setRequests(QList<int> requestsl);
    void addFriend(int friendId);
    void removeFriend(int friendId);

private:
    QString name;
    QString username;
    QString description;
    int id;
    QList<int> friends;
    QList<int> recommended;
    QList<int> requests;

};



#endif // USER_H
