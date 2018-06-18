#ifndef BLACKDIALOG_H
#define BLACKDIALOG_H

#include <QDialog>

namespace Ui {
class BlackDialog;
}

class BlackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BlackDialog(QWidget *parent = 0);
    ~BlackDialog();

    void setLeftTime(QString &str);

private:
    Ui::BlackDialog *ui;
};

#endif // BLACKDIALOG_H
