#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

}



void Dialog::on_buttonBox_accepted()
{
    qDebug() << "click dialog 确定";
}


void Dialog::on_buttonBox_rejected()
{
    qDebug() << "click dialog 取消";
}



Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_dial_valueChanged(int value)
{
    ui->lcdNumber->display(value);
}


void Dialog::on_horizontalSlider_valueChanged(int value)
{
    ui->progressBar->setValue(value);
}

