#ifndef FANSETQT_H
#define FANSETQT_H

#include <QWidget>
#include "fancontroller.h"

namespace Ui {
class FanSetQt;
}

class FanSetQt : public QWidget
{
    Q_OBJECT

public:
    explicit FanSetQt(QWidget *parent = 0);
    ~FanSetQt();

private slots:
    void on_FanOnButton_clicked();

    void on_FanOffButton_clicked();

private:
    Ui::FanSetQt *ui;
    FanController *fc;
    int state;
};

#endif // FANSETQT_H
