#include "settings.h"
#include <QLabel>
#include <QPushButton>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QPalette>
#include <QComboBox>
#include <QDebug>
#include <QString>

// function to add a divider
void addHorizontalDivider(QVBoxLayout *layout) {
    QFrame *divider = new QFrame();
    divider->setFrameShape(QFrame::HLine);
    divider->setFrameShadow(QFrame::Sunken);
    layout->addWidget(divider);
}

// function to change all the strings that should really change when you change language
void Settings::changeLanguage(const QString& newLanguage) {
    selectedLanguage = newLanguage;
    qDebug() << "Language changed to: " << selectedLanguage;

    if (selectedLanguage == "English") {
        currentLoginString = "Currently logged in as:";
        editString = "Edit Profile";
        logoutString = "Log Out";
        languageString = "Language Settings";
        applyString = "Apply";
        themeString = "Theme";
        lightString = "Light Mode";
        darkString = "Dark Mode";
        saveString = "Save";
        cancelString = "Cancel";
    }
    else if (selectedLanguage == "Spanish") {
        currentLoginString = "Actualmente conectado como:";
        editString = "Editar Perfil";
        logoutString = "Cerrar Sesión";
            languageString = "Configuración de Idioma";
            themeString = "Tema";
        lightString = "Modo Claro";
        darkString = "Modo Oscuro";
        applyString = "Aplicar";
        saveString = "Guardar";
        cancelString = "Cancelar";
    }
    else if (selectedLanguage == "Chinese") {
        currentLoginString = "当前登录用户：";
        editString = "编辑个人资料";
        logoutString = "退出";
        languageString = "语言设置";
        themeString = "主题";
        lightString = "亮色模式";
        darkString = "暗色模式";
        applyString = "应用";
        saveString = "保存";
        cancelString = "取消";
    }

    qDebug() << "User Label: " << currentLoginString + ": " + user;
    qDebug() << "Edit Profile Button: " << editString;
    qDebug() << "Log Out Button: " << logoutString;
    qDebug() << "Language Label: " << languageString;
    qDebug() << "Theme Label: " << themeString;
    qDebug() << "Light label" << lightString;
    qDebug() << "Dark label" << darkString;
    qDebug() << "Apply Button: " << applyString;
    qDebug() << "Save Button: " << saveString;
    qDebug() << "Cancel Button: " << cancelString;
}

// this function is called when the user updates the strings from the previous function
// basically just updates the UI
// because it isnt updated just by updating the strings
void Settings::updateUI() {

    // updating label
    userLabel->setText(currentLoginString + " " + user);

    // updating profile settings
    editProfileButton->setText(editString);
    logOutButton->setText(logoutString);

    // updating language settings
    languageLabel->setText(languageString);
    languageButton->setText(applyString);

    // updating theme settings
    themeLabel->setText(themeString);
    themeComboBox->clear();
    themeComboBox->addItem(darkString);
    themeComboBox->addItem(lightString);
    themeButton->setText(applyString);

    // updating exit buttons
    saveButton->setText(saveString);
    cancelButton->setText(cancelString);
}

void Settings::changeTheme() {

    if (!themeComboBox) {
        qDebug() << "Error: themeComboBox is null.";
        return;
    }

    QPalette newPalette;

    if (themeComboBox->currentText() == darkString) {
        newPalette.setColor(QPalette::Background, QColor("#001452")); // background colour
        this->setStyleSheet("color: #9c9c9c;"); // text colour
        qDebug() << "set theme to dark";
    }
    if (themeComboBox->currentText() == lightString) {
        newPalette.setColor(QPalette::Background, QColor("#ffffff")); // background colour
        this->setStyleSheet("color: black;");
        qDebug() << "set theme to light";
    }

    this->setAutoFillBackground(true);
    this->setPalette(newPalette);
}

Settings::Settings(QWidget *parent) : QWidget(parent)
{
    // initialise language as english
    changeLanguage("English");
    user = "josh 123";

    // initialise the page layout
    // initialise variable for location of the settings
    QRect offscreenRect(-width(), 0, width(), height());
    setGeometry(offscreenRect);
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop);

    // Heading
    QLabel *titleLabel = new QLabel("Tomeo", this);
    titleLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    titleLabel->setFont(QFont("Arial", 20));
    mainLayout->addWidget(titleLabel);

    addHorizontalDivider(mainLayout);

    // User Details
    qDebug() << "UI user label" << currentLoginString;
    userLabel = new QLabel(currentLoginString + " " + user, this);
    mainLayout->addWidget(userLabel);

    // create horizontal box for user buttons
    QHBoxLayout *userBox = new QHBoxLayout();
    editProfileButton = new QPushButton(editString, this);
    logOutButton = new QPushButton(logoutString, this);
    for (auto button : {editProfileButton, logOutButton}) {
        button->setMinimumSize(60, 25);
        userBox->addWidget(button);
        button->setStyleSheet("color: black;");
    }
    mainLayout->addLayout(userBox);

    addHorizontalDivider(mainLayout);

    // Language Settings
    languageLabel = new QLabel(languageString, this);
    languageComboBox = new QComboBox(this);
    languageComboBox->addItem("English");
    languageComboBox->addItem("Spanish");
    languageComboBox->addItem("Chinese");
    languageComboBox->setStyleSheet("color: black");

    languageButton = new QPushButton(applyString, this);
    languageButton->setMinimumSize(60, 25);
    languageButton->setStyleSheet("color: black;");

    mainLayout->addWidget(languageLabel);
    mainLayout->addWidget(languageComboBox);
    mainLayout->addWidget(languageButton);

    addHorizontalDivider(mainLayout);

    // theme settings
    themeLabel = new QLabel(themeString, this);
    themeComboBox = new QComboBox(this);
    themeComboBox->addItem(darkString);
    themeComboBox->addItem(lightString);
    themeComboBox->setStyleSheet("color: black");
    changeTheme();

    themeButton = new QPushButton(applyString, this);
    themeButton->setMinimumSize(60, 25);
    themeButton->setStyleSheet("color: black;");

    mainLayout->addWidget(themeLabel);
    mainLayout->addWidget(themeComboBox);
    mainLayout->addWidget(themeButton);

    addHorizontalDivider(mainLayout);

    // create horizontal box for exit buttons
    QHBoxLayout *exitBox = new QHBoxLayout();
    saveButton = new QPushButton(saveString, this);
    cancelButton = new QPushButton(cancelString, this);
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
        animationOut->setDuration(500);
        animationOut->setStartValue(geometry());
        animationOut->setEndValue(QRect(-width(), 0, width(), height()));
        animationOut->start();

        QObject::connect(animationOut, &QPropertyAnimation::finished, [=]() {
            hide();
        });
    };

    // connecting the buttons to the functions
    QObject::connect(saveButton, &QPushButton::clicked, slideOutAnimation);
    QObject::connect(cancelButton, &QPushButton::clicked, slideOutAnimation);

    // connect button to change language
    QObject::connect(languageButton, &QPushButton::clicked, [=]() {
        changeLanguage(languageComboBox->currentText());
    });
    QObject::connect(languageButton, &QPushButton::clicked, this, &Settings::updateUI);

    // connect button to change theme
    QObject::connect(themeButton, &QPushButton::clicked, this, [=](){
        changeTheme();
    });

    hide();
}
