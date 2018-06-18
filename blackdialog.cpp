#include "blackdialog.h"
#include "ui_blackdialog.h"

#include <QApplication>
#include <QScreen>

BlackDialog::BlackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackDialog)
{
    ui->setupUi(this);
    setGeometry(QApplication::screens().at(0)->geometry());
    setWindowFlags(Qt::FramelessWindowHint);
}

BlackDialog::~BlackDialog()
{
    delete ui;
}

void BlackDialog::setLeftTime(QString &str)
{
    ui->timeLabel->setText(str);
}
