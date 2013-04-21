#include "dialogconfig.h"
#include "ui_dialogconfig.h"

DialogConfig::DialogConfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogConfig)
{
    ui->setupUi(this);

    settings = new QSettings("setting.ini", QSettings::IniFormat, this);
    ui->lineEditAddress->setText(settings->value("address").toString());
    ui->lineEditPort->setText(settings->value("port").toString());
}

DialogConfig::~DialogConfig()
{
    delete ui;
}

void DialogConfig::on_buttonBox_accepted()
{
    settings->setValue("address", ui->lineEditAddress->text());
    settings->setValue("port", ui->lineEditPort->text());
}
