#include "popup.h"
#include "ui_popup.h"

#include <QTimer>

Popup::Popup(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Popup);

    QTimer::singleShot(3000, this, SLOT(close()));
}

Popup::~Popup()
{
    delete ui;
}
