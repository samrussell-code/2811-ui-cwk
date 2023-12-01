#include "settings.h"
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QPalette>


Settings::Settings(QWidget *parent) : QWidget(parent)
{
    // initialise colour scheme
    QPalette palette = this->palette();
    palette.setColor(QPalette::Background, QColor("#001452")); // background colour
    this->setAutoFillBackground(true); // background fill
    this->setPalette(palette);
    this->setStyleSheet("color: #9c9c9c;"); // text colour

    // initialise the page layout
    // initialise variable for location of the settings
    QRect offscreenRect(-width(), 0, width(), height());
    setGeometry(offscreenRect);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // Heading
    QLabel *titleLabel = new QLabel("Settings:", this);
    titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    titleLabel->setFont(QFont("Arial", 20));
    mainLayout->addWidget(titleLabel);

    // User Details
    QLabel *userLabel = new QLabel("User: josh123", this);
    mainLayout->addWidget(userLabel);

    // create horizontal box for user buttons
    QHBoxLayout *userBox = new QHBoxLayout();
    QPushButton *editProfileButton = new QPushButton("Edit Profile", this);
    QPushButton *logOutButton = new QPushButton("Log Out", this);
    for (auto button : {editProfileButton, logOutButton}) {
        button->setMinimumSize(60, 25);
        userBox->addWidget(button);
        button->setStyleSheet("color: black;");
    }
    mainLayout->addLayout(userBox);

    // Language Settings
    QLabel *languageLabel = new QLabel("Language Settings:", this);
    QPushButton *languageButton = new QPushButton("Language", this);
    languageButton->setMinimumSize(60, 25);
    languageButton->setStyleSheet("color: black;");

    mainLayout->addWidget(languageLabel);
    mainLayout->addWidget(languageButton);

    // create horizontal box for exit buttons
    QHBoxLayout *exitBox = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save Settings", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    for (auto button : {saveButton, cancelButton}) {
        button->setMinimumSize(60, 25);
        exitBox->addWidget(button);
        button->setStyleSheet("color: black;");
    }
    mainLayout->addLayout(exitBox);

    // set the layout of the widget
    setLayout(mainLayout);

    // animation to close the settings
    auto slideOutAnimation = [=]() {

        QPropertyAnimation *animationOut = new QPropertyAnimation(this, "geometry");
        animationOut->setDuration(500); // Set the duration of the animation in milliseconds
        animationOut->setStartValue(geometry());
        animationOut->setEndValue(QRect(-width(), 0, width(), height()));
        animationOut->start();

        QObject::connect(animationOut, &QPropertyAnimation::finished, [=]() {
            hide();
        });
    };

    QObject::connect(saveButton, &QPushButton::clicked, slideOutAnimation);
    QObject::connect(cancelButton, &QPushButton::clicked, slideOutAnimation);

    // automatically hide on startup
    hide();
}
