#include "faqlayout.h"
#include <QtWidgets>

FaqLayout::FaqLayout(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    setStyleSheet("background-color: white;");

    QLabel *titleLabel = new QLabel("Frequently Asked Questions");
    titleLabel->setStyleSheet("background-color: black; color: white; font-size: 24px; padding: 10px; text-align: center;");

    mainLayout->addWidget(titleLabel);

    QGroupBox *faqGroup = new QGroupBox("FAQ");
    QVBoxLayout *faqLayout = new QVBoxLayout(faqGroup);

    // Add FAQ entries with labels and descriptions
    addFaqEntry(faqLayout, "What is the purpose of this app?", "Tomeo is a video player app designed for sports enthusiasts to easily browse and watch sports-related videos.");

    addFaqEntry(faqLayout, "How do I search for users?", "Use the search bar at the top to enter the username or keywords to find friends.");

    addFaqEntry(faqLayout, "What do the icons mean?", "View Profile - Opens the profile of the selected friend.\nAdd Friend - Adds the selected recommended friend.\nAccept - Accepts a friend request.\nDecline - Declines a friend request.");

    // Add more FAQ entries as needed

    mainLayout->addWidget(faqGroup);
}

void FaqLayout::addFaqEntry(QVBoxLayout *layout, const QString &question, const QString &answer) {
    QHBoxLayout *faqEntryLayout = new QHBoxLayout;

    QLabel *questionLabel = new QLabel(question);
    questionLabel->setStyleSheet("font-weight: bold;");

    QLabel *answerLabel = new QLabel(answer);

    faqEntryLayout->addWidget(questionLabel);
    faqEntryLayout->addWidget(answerLabel);

    layout->addLayout(faqEntryLayout);
}
