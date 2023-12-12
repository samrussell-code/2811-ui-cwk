// faqlayout.cpp
#include "faqlayout.h"
#include <QtWidgets>

FaqLayout::FaqLayout(QWidget *parent) : QWidget(parent) {
    QString buildPath = QCoreApplication::applicationDirPath();

    // setup icon paths
    QString settingsIconFilename = "settings.png";
    QString settingsIconPath = QDir(buildPath).filePath("icons/" + settingsIconFilename);
    QString profileIconFilename = "profile_icon.png";
    QString profileIconPath = QDir(buildPath).filePath("icons/" + profileIconFilename);
    QString recordVideoIconFilename = "camera.png";
    QString recordVideoIconPath = QDir(buildPath).filePath("icons/" + recordVideoIconFilename);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    setStyleSheet("background-color: white;");

    QLabel *titleLabel = new QLabel("Frequently Asked Questions");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 24px; padding: 10px; text-align: center;");
    titleLabel->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    mainLayout->addWidget(titleLabel);

    QGroupBox *faqGroup = new QGroupBox("FAQ");
    QVBoxLayout *faqLayout = new QVBoxLayout(faqGroup);

    addFaqEntry(faqLayout, "What is the purpose of this app?", "Tomeo is a social media video platform where you can post a video at the same time as your friends each day and see what everyone is really up to at a scheduled time of day!", QString());

    addFaqEntry(faqLayout, "Why does it have a daily schedule?", "We want to provide a window into what your typical day looks like, and the schedule timer is just one way of shaking up what you might see on your feed!\n ", QString());

    addFaqEntry(faqLayout, "How do I search for users?", "In the friends tab - there is a search bar on the page which will allow you to add users based on name, username, or phone number.", QString());

    addFaqEntry(faqLayout, "Icons Explained:", "", QString());

    addFaqEntry(faqLayout, "Settings", "Opens the settings page with a selection of accessibility settings, themes, and languages. You can manage account settings here too.", settingsIconPath);

    addFaqEntry(faqLayout, "Record Video", "This is your camera section - here you can record a daily video, upload that video, and keep track of the daily schedule so you know when to check for your friends' videos.", recordVideoIconPath);

    addFaqEntry(faqLayout, "Friends", "Here you can add new people that you want to see in your feed and view the profiles of other friends! You can manage your friendship status here.", profileIconPath);

    mainLayout->addWidget(faqGroup);

    QFont faqFont = font();
    faqFont.setPointSize(faqFont.pointSize() + 2);
    titleLabel->setFont(faqFont);

    for (int i = 0; i < faqLayout->count(); ++i) {
        QLayoutItem *item = faqLayout->itemAt(i);
        if (item->layout()) {
            for (int j = 0; j < item->layout()->count(); ++j) {
                if (QLabel *label = qobject_cast<QLabel *>(item->layout()->itemAt(j)->widget())) {
                    label->setFont(faqFont); //answer labels have to be wrapped so they dont overflow on screen
                    label->setWordWrap(true);
                }
            }
        }
    }
}

void FaqLayout::addFaqEntry(QVBoxLayout *layout, const QString &question, const QString &answer, const QString &iconPath) {
    QHBoxLayout *faqEntryLayout = new QHBoxLayout;

    if (!iconPath.isEmpty()) {
        QLabel *iconLabel = new QLabel;
        iconLabel->setPixmap(QPixmap(iconPath).scaled(70, 70, Qt::KeepAspectRatio));
        faqEntryLayout->addWidget(iconLabel);
    }

    QLabel *questionLabel = new QLabel(question);
    questionLabel->setStyleSheet("font-weight: bold;");
    faqEntryLayout->addWidget(questionLabel);

    QLabel *answerLabel = new QLabel(answer);
    faqEntryLayout->addWidget(answerLabel);

    layout->addLayout(faqEntryLayout);
}
