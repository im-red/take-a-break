#pragma once

#include <QWidget>
#include <QSystemTrayIcon>
#include <QTime>

class BlackDialog;

class QTimerEvent;
class QMenu;
class QAction;
class QCloseEvent;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void timerEvent(QTimerEvent *event) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void setNewDuration();
    void resetLeftDuration();
    void onRestActionTriggered();
    void onBlackDialogHidden();

private:
    void enterRest();
    void leaveRest();
    void createActions();
    void createTrayIcon();
    void updateToolTip();
    enum E_State
    {
        E_INIT,
        E_WORK,
        E_REST,
        E_PAUSE
    };

    Ui::Widget *ui;

    E_State m_state;

    QTime m_totalWork;
    QTime m_totalRest;
    QTime m_leftWork;
    QTime m_leftRest;
    QTime m_zeroTime;

    QAction *m_restAction;
    QAction *m_openAction;
    QAction *m_quitAction;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayMenu;
    BlackDialog *m_blackDialog;
};
