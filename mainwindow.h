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
//    void on_m_btn_AWF_clicked(bool checked);
    void on_m_btn_hsvpro_clicked(bool checked);
    void on_m_btn_Bchannel_clicked(bool checked);
    void on_m_btn_binarythre_clicked(bool checked);
    void on_m_btn_binarythre_inv_clicked(bool checked);
    void on_m_btn_thtr_clicked(bool checked);
    void on_m_btn_thtoinv_clicked(bool checked);
    void on_m_btn_thto_clicked(bool checked);
    void on_m_btn_adTh_clicked(bool checked);
    void on_m_btn_Gchannel_clicked(bool checked);
    void on_m_btn_Rchannel_clicked(bool checked);
    void on_m_btn_avblur_clicked(bool checked);
    void on_m_btn_bofil_clicked(bool checked);
    void on_m_btn_gabl_clicked(bool checked);
    void on_m_btn_mebl_clicked(bool checked);
    void on_m_btn_bifi_clicked(bool checked);
    void on_m_btn_2Dfi_clicked(bool checked);



//滑动条
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_2_valueChanged(int value);
    void on_horizontalSlider_3_valueChanged(int value);
    void on_horizontalSlider_4_valueChanged(int value);
    void on_horizontalSlider_5_valueChanged(int value);
    void on_horizontalSlider_6_valueChanged(int value);
};
#endif // MAINWINDOW_H
