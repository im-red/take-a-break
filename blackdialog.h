#ifndef BLACKDIALOG_H
#define BLACKDIALOG_H

#include <QDialog>
#include <QHideEvent>

namespace Ui {
class BlackDialog;
}

class BlackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlackDialog(QWidget *parent = 0);
    ~BlackDialog();

    void setLeftTime(const QString &str);

signals:
    void hidden();

protected:
    void hideEvent(QHideEvent *e) override;

private slots:
    void on_exitButton_clicked();

private:
    Ui::BlackDialog *ui;
};

#endif // BLACKDIALOG_H
