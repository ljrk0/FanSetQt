#include "aboutfrm.h"
#include "ui_aboutfrm.h"

AboutFrm::AboutFrm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutFrm)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog);
}

AboutFrm::~AboutFrm()
{
    delete ui;
}

void AboutFrm::on_CloseButton_clicked()
{
    this->destroy();
}
