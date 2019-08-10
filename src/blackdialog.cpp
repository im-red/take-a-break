#include "blackdialog.h"
#include "ui_blackdialog.h"

#include <QApplication>
#include <QScreen>
#include <QDesktopWidget>
#include <QDebug>
#include <QEvent>

BlackDialog::BlackDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BlackDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint
                   | Qt::BypassWindowManagerHint
                   | Qt::WindowStaysOnTopHint
                   | Qt::WindowDoesNotAcceptFocus);
}

BlackDialog::~BlackDialog()
{
    delete ui;
}

void BlackDialog::setRemainingTime(const QString &str)
{
    ui->timeLabel->setText(str);
}

bool BlackDialog::event(QEvent *e)
{
    // qDebug() << e;
    return QDialog::event(e);
}

void BlackDialog::closeEvent(QCloseEvent *e)
{
    e->ignore();
}

void BlackDialog::showEvent(QShowEvent *)
{
    setGeometry(QApplication::desktop()->geometry());
    ui->timeLabel->setGeometry(QApplication::primaryScreen()->geometry());
}

void BlackDialog::hideEvent(QHideEvent *)
{
    emit hidden();
}

void BlackDialog::keyPressEvent(QKeyEvent *e)
{
    e->accept();
}

void BlackDialog::on_exitButton_clicked()
{
    hide();
}
