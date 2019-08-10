/*********************************************************************************
 * MIT License
 *
 * Copyright (c) 2019 Jia Lihong
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ********************************************************************************/

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
