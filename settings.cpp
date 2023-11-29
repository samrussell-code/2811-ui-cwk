// settingspage.cpp
#include "settings.h"
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>

    Settings::Settings(QWidget *parent) : QWidget(parent)
{
    // Set up your settings page UI here
    // For example, add labels, buttons, and other widgets.
    // You can use layouts to organize the widgets.

    QLabel *titleLabel = new QLabel("Settings Page", this);
    titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleLabel->setFont(titleFont);

    QLabel *userLabel = new QLabel("User:", this);
    QFont userLabelFont = userLabel->font();
    userLabelFont.setPointSize(10);  // Set the font size for userLabel only
    userLabel->setFont(userLabelFont);


    QPushButton *saveButton = new QPushButton("Save Settings", this);
    saveButton->setMinimumSize(60, 25); // Set the minimum size for saveButton

    QLabel *LanguageLabel = new QLabel("Language Settings:", this);
    QPushButton *language = new QPushButton("Language", this);
    language->setMinimumSize(60, 25);   // Set the minimum size for language

    QLabel *EditLabel = new QLabel("Edit Profile", this);
    QPushButton *editProfile = new QPushButton("Edit Profile", this);
    editProfile->setMinimumSize(60, 25);

    // Example: Using a layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(userLabel);

    // Add buttons to the button layout
    mainLayout->addWidget(EditLabel);
    mainLayout->addWidget(editProfile);

    mainLayout->addWidget(LanguageLabel);
    mainLayout->addWidget(language);

    mainLayout->addWidget(saveButton);
    mainLayout->addStretch();

    // Set the main layout for the SettingsPage
    setLayout(mainLayout);

    hide();
}
