#pragma once

#include <QDialog>
#include <QHideEvent>

namespace Ui {
class BlackDialog;
}

class BlackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlackDialog(QWidget *parent = nullptr);
    ~BlackDialog() override;

    void setLeftTime(const QString &str);

signals:
    void hidden();

protected:
    bool event(QEvent *e) override;
    void closeEvent(QCloseEvent *e) override;
    void showEvent(QShowEvent *) override;
    void hideEvent(QHideEvent *) override;
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void on_exitButton_clicked();

private:
    Ui::BlackDialog *ui;
};
