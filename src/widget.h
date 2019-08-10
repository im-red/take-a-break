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
