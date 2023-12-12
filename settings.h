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
    void changeTextScaling();
    void changeFontSize(int newSize);

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
    QString textScalingString;
    QString smallString;
    QString mediumString;
    QString largeString;
    QString saveString;
    QString cancelString;

    QLabel *userLabel;
    QLabel *languageLabel;
    QLabel *themeLabel;
    QLabel *textScalingLabel;
    QComboBox *languageComboBox;
    QComboBox *themeComboBox;
    QComboBox *textScalingComboBox;
    QPushButton *editProfileButton;
    QPushButton *logOutButton;
    QPushButton *languageButton;
    QPushButton *textScalingButton;
    QPushButton *themeButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;


signals:
    void themeChanged(const QString& theme);
    void openProfile();

public slots:
};

#endif
