#ifndef ABOUTFRM_H
#define ABOUTFRM_H

#include <QWidget>

namespace Ui {
class AboutFrm;
}

class AboutFrm : public QWidget
{
    Q_OBJECT

public:
    explicit AboutFrm(QWidget *parent = 0);
    ~AboutFrm();

private slots:
    void on_CloseButton_clicked();

private:
    Ui::AboutFrm *ui;
};

#endif // ABOUTFRM_H
