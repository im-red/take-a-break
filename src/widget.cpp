#include "widget.h"
#include "ui_widget.h"
#include "blackdialog.h"

#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QCloseEvent>
#include <QDebug>
#include <QWindow>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , m_state(Init)
{
    ui->setupUi(this);
    m_blackDialog.reset(new BlackDialog());

    connect(ui->timeButton, &QPushButton::clicked, this, &Widget::setNewDuration);
    connect(m_blackDialog.get(), &BlackDialog::hidden, this, &Widget::onBlackDialogHidden);

    createTrayIcon();
    m_zeroTime.setHMS(0, 0, 0);
    startTimer(1000);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::timerEvent(QTimerEvent *)
{
    switch (m_state)
    {
    case Init:
    {
        setNewDuration();
        resetRemainingDuration();
        m_state = Working;
        break;
    }
    case Working:
    {
        if (m_remainingWork == m_zeroTime)
        {
            resetRemainingDuration();
            m_state = Resting;
        }
        else
        {
            m_remainingWork = m_remainingWork.addSecs(-1);
        }

        break;
    }
    case Resting:
    {
        if (m_remainingRest == m_zeroTime)
        {
            resetRemainingDuration();
            m_state = Working;
            leaveRest();
        }
        else
        {
            if (m_remainingRest == m_totalRest)
            {
                enterRest();
            }
            m_remainingRest = m_remainingRest.addSecs(-1);
        }
        break;
    }
    }
    updateToolTip();
}

void Widget::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason)
    {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    {
        showNormal();
        break;
    }
    default:
        break;
    }
}

void Widget::setNewDuration()
{
    m_totalWork.setHMS(ui->workSpinBox->value() / 60, ui->workSpinBox->value() % 60, 0);
    m_totalRest.setHMS(ui->restSpinBox->value() / 60, ui->restSpinBox->value() % 60, 0);
    resetRemainingDuration();
}

void Widget::resetRemainingDuration()
{
    m_remainingWork = m_totalWork;
    m_remainingRest = m_totalRest;
}

void Widget::onRestActionTriggered()
{
    resetRemainingDuration();
    m_state = Resting;
}

void Widget::onBlackDialogHidden()
{
    resetRemainingDuration();
    m_state = Working;
}

void Widget::enterRest()
{
    m_blackDialog->show();
}

void Widget::leaveRest()
{
    m_blackDialog->hide();
}

void Widget::createActions()
{
    m_restAction = new QAction(tr("Rest"), this);
    connect(m_restAction, &QAction::triggered, this, &Widget::onRestActionTriggered);

    m_openAction = new QAction(tr("Open"), this);
    connect(m_openAction, &QAction::triggered, this, &QWidget::showNormal);

    m_quitAction = new QAction(tr("Quit"), this);
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);
}

void Widget::createTrayIcon()
{
    createActions();

    m_trayMenu = new QMenu(this);
    m_trayMenu->addAction(m_restAction);
    m_trayMenu->addSeparator();
    m_trayMenu->addAction(m_openAction);
    m_trayMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &Widget::iconActivated);

    QIcon icon(":/icon.png");
    m_trayIcon->setIcon(icon);
    setWindowIcon(icon);
    m_trayIcon->show();
}

void Widget::updateToolTip()
{
    QString toolTip;
    switch (m_state)
    {
    case Working:
    {
        toolTip = "Working ";
        toolTip += m_remainingWork.toString("hh:mm:ss");
        break;
    }
    case Resting:
    {
        toolTip = "Resting ";
        toolTip += m_remainingRest.toString("hh:mm:ss");
        m_blackDialog->setRemainingTime(toolTip);
        break;
    }
    default:
    {
        return;
    }
    }

    m_trayIcon->setToolTip(toolTip);
    ui->tipLabel->setText(toolTip);
}
