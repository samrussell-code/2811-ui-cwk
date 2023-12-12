#ifndef FAQLAYOUT_H
#define FAQLAYOUT_H

#include "qboxlayout.h"
#include <QWidget>

class FaqLayout : public QWidget {
    Q_OBJECT

public:
    FaqLayout(QWidget *parent = nullptr);

private:
    void addFaqEntry(QVBoxLayout *layout, const QString &question, const QString &answer);
};

#endif // FAQLAYOUT_H
