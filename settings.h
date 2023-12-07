#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPalette>>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    void changeLanguage(const QString& newLanguage);
    void updateUI();
    void changeTheme();


private:
    QPalette palette;

    QString selectedLanguage;
    QString user;
    QString currentLoginString;
    QString editString;
    QString logoutString;
    QString languageString;
    QString applyString;
    QString themeString;
    QString lightString;
    QString darkString;
    QString saveString;
    QString cancelString;

    QLabel *userLabel;
    QLabel *languageLabel;
    QLabel *themeLabel;
    QComboBox *languageComboBox;
    QComboBox *themeComboBox;
    QPushButton *editProfileButton;
    QPushButton *logOutButton;
    QPushButton *languageButton;
    QPushButton *themeButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;


signals:

public slots:
};

#endif
