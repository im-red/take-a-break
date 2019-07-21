#include "blackdialog.h"
#include "ui_blackdialog.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>

BlackDialog::BlackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackDialog)
{
    ui->setupUi(this);
    setGeometry(QApplication::desktop()->geometry());
    ui->timeLabel->setGeometry(QApplication::primaryScreen()->geometry());
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::BypassWindowManagerHint
                   | Qt::WindowStaysOnTopHint);
}

BlackDialog::~BlackDialog()
{
    delete ui;
}

void BlackDialog::setLeftTime(const QString &str)
{
    ui->timeLabel->setText(str);
}

void BlackDialog::hideEvent(QHideEvent *e)
{
    (void) e;
    emit hidden();
}

void BlackDialog::on_exitButton_clicked()
{
    hide();
}
