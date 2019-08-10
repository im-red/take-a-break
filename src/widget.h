#pragma once

#include <QWidget>
#include <QSystemTrayIcon>
#include <QTime>

#include <memory>

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
    explicit Widget(QWidget *parent = nullptr);
    ~Widget() override;

protected:
    void timerEvent(QTimerEvent *) override;
    void closeEvent(QCloseEvent *event) override;

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void setNewDuration();
    void resetRemainingDuration();
    void onRestActionTriggered();
    void onBlackDialogHidden();

private:
    void enterRest();
    void leaveRest();
    void createActions();
    void createTrayIcon();
    void updateToolTip();
    enum State
    {
        Init,
        Working,
        Resting
    };

    Ui::Widget *ui;

    State m_state = Init;

    QTime m_totalWork;
    QTime m_totalRest;
    QTime m_remainingWork;
    QTime m_remainingRest;
    QTime m_zeroTime;

    QAction *m_restAction = nullptr;
    QAction *m_openAction = nullptr;
    QAction *m_quitAction = nullptr;
    QSystemTrayIcon *m_trayIcon = nullptr;
    QMenu *m_trayMenu = nullptr;

    std::unique_ptr<BlackDialog> m_blackDialog;
};
