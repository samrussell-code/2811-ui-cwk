// faqlayout.h
#ifndef FAQLAYOUT_H
#define FAQLAYOUT_H

#include <QWidget>
#include <QtWidgets>

class FaqLayout : public QWidget {
    Q_OBJECT

public:
    explicit FaqLayout(QWidget *parent = nullptr);

private:
    void addFaqEntry(QVBoxLayout *layout, const QString &question, const QString &answer, const QString &iconPath);
};

#endif // FAQLAYOUT_H
