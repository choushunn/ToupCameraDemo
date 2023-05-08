#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qtimer.h"
#include "appinit.h"
#include "appevent.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    AppInit*    appInit;
    AppEvent*   appEvent;
    Ui::MainWindow *ui;
    QTimer*     m_timer = nullptr;
    int         fps = 30;
    void onTimer();

private slots:
    void on_m_btn_open_camera_clicked(bool checked);
    void showFrame(QImage image1, QImage image2);
    void on_m_btn_graypro_clicked(bool checked);
    void on_m_btn_hflip_clicked(bool checked);
    void on_m_btn_Hist_clicked(bool checked);
    void on_m_btn_rgbHist_clicked(bool checked);
};
#endif // MAINWINDOW_H
