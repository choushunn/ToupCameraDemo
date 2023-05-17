#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private:
    Ui::Dialog *ui;

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void on_dial_valueChanged(int value);
    void on_horizontalSlider_valueChanged(int value);
};

#endif // DIALOG_H
