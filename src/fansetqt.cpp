#include "fansetqt.h"
#include "ui_fansetqt.h"

FanSetQt::FanSetQt(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FanSetQt)
{
    this->setWindowFlags(Qt::Dialog);

    fc = new FanController;

    trayMenu = new QMenu(this);

    FanOnAction = new QAction("Fan On",trayMenu);
    FanOffAction = new QAction("Fan Off", trayMenu);
    AboutAction = new QAction("About",trayMenu);

    connect(FanOnAction, SIGNAL(triggered(bool)), this, SLOT(on_FanOnButton_clicked()));
    connect(FanOffAction, SIGNAL(triggered(bool)), this, SLOT(on_FanOffButton_clicked()));
    connect(AboutAction, SIGNAL(triggered(bool)), this, SLOT(ShowAbout()));

    trayMenu->addAction(FanOnAction);
    trayMenu->addAction(FanOffAction);
    trayMenu->addAction(AboutAction);

    QIcon AppIcon("fan8.svg");
    trayIcon = new QSystemTrayIcon(AppIcon,this);

    this->setWindowIcon(AppIcon);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->setVisible(true);

    ui->setupUi(this);

#ifdef _MSC_VER
    ui->InfoLabel->setText(QString::fromStdString("Hint: This application needs to be run as administrator!"));
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

void FanSetQt::ShowAbout()
{
    AboutScreen = new AboutFrm(this);
    QIcon AppIcon("fan8.svg");
    AboutScreen->setWindowIcon(AppIcon);
    AboutScreen->show();
}
