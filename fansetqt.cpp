#include "fansetqt.h"
#include "ui_fansetqt.h"

FanSetQt::FanSetQt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FanSetQt)
{
    ui->setupUi(this);
    fc = new FanController;

#ifdef _MSC_VER
    ui->InfoLabel->text().fromStdString("Hint: This application needs to be run as administrator!");
#else
    ui->InfoLabel->setText(QString::fromStdString("Hint: This application needs to be run as root!"));
#endif

}

FanSetQt::~FanSetQt()
{
    delete ui;
}

void FanSetQt::on_FanOnButton_clicked()
{
    fc->setFan(true);
}

void FanSetQt::on_FanOffButton_clicked()
{
    fc->setFan(false);
}
