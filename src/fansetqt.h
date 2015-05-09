#ifndef FANSETQT_H
#define FANSETQT_H

#include <QWidget>
#include "fancontroller.h"
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <aboutfrm.h>

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

    void ShowAbout();

private:
    Ui::FanSetQt *ui;

    FanController *fc;
//    int state;
    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAction *FanOnAction;
    QAction *FanOffAction;
    QAction *AboutAction;
    AboutFrm *AboutScreen;
};

#endif // FANSETQT_H
