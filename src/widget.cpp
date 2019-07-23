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
    , m_state(E_INIT)
{
    ui->setupUi(this);
    m_blackDialog = new BlackDialog();

    connect(ui->timeButton, &QPushButton::clicked, this, &Widget::setNewDuration);
    connect(m_blackDialog, &BlackDialog::hidden, this, &Widget::onBlackDialogHidden);

    createTrayIcon();
    m_zeroTime.setHMS(0, 0, 0);
    setNewDuration();
    startTimer(1000);
}

Widget::~Widget()
{
    delete ui;
    delete m_blackDialog;
}

void Widget::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event)

    switch (m_state) {
    case E_INIT:
    {
        setNewDuration();
        resetLeftDuration();
        m_state = E_WORK;
        break;
    }
    case E_WORK:
    {
        if (m_leftWork == m_zeroTime)
        {
            resetLeftDuration();
            m_state = E_REST;
        }
        else
        {
            m_leftWork = m_leftWork.addSecs(-1);
        }

        break;
    }
    case E_REST:
    {
        if (m_leftRest == m_zeroTime)
        {
            resetLeftDuration();
            m_state = E_WORK;
            leaveRest();
        }
        else
        {
            if (m_leftRest == m_totalRest)
            {
                enterRest();
            }
            m_leftRest = m_leftRest.addSecs(-1);
        }
    }
    case E_PAUSE:
    {
        break;
    }
    default:
        break;
    }
    updateToolTip();
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (ui->minRadioButton->isChecked())
    {
        hide();
        event->ignore();
    }
    else if (ui->closeRadioButton->isChecked())
    {
        qApp->setQuitOnLastWindowClosed(true);
        event->accept();
    }
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
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
    resetLeftDuration();
}

void Widget::resetLeftDuration()
{
    m_leftWork = m_totalWork;
    m_leftRest = m_totalRest;
}

void Widget::onRestActionTriggered()
{
    resetLeftDuration();
    m_state = E_REST;
}

void Widget::onBlackDialogHidden()
{
    resetLeftDuration();
    m_state = E_WORK;
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
    switch (m_state) {
    case E_WORK:
    {
        toolTip = "Working ";
        toolTip += m_leftWork.toString("hh:mm:ss");
        break;
    }
    case E_REST:
    {
        toolTip = "Resting ";
        toolTip += m_leftRest.toString("hh:mm:ss");
        m_blackDialog->setLeftTime(toolTip);
        break;
    }
    case E_PAUSE:
    default:
    {
        return;
        break;
    }
    }

    m_trayIcon->setToolTip(toolTip);
    ui->tipLabel->setText(toolTip);
}
