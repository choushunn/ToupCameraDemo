#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer.h>
#include <DeviceManager.h>
#include "appinit.h"
#include "appevent.h"
#include "dialog.h"
#include "ImageProcessor.h"
#include <vector>

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
    Ui::MainWindow *ui;
    AppInit m_appInit;
    AppEvent &m_appEvent = AppEvent::instance();
    DeviceManager m_deviceManager;
    void setupUI();
    void setDefaultValues();
    void setDefaultStates();
    void registerEvents();
    void unregisterEvents();
    void signalSlotConnect();

    void getCameraList();
    int m_camIndex = 0;
    std::string m_camType = "USB";
    QTimer* m_readTimer;
    int m_fps = 30;
private slots:
    void handleEvent(const QString& eventName, const QVariant& eventData);
    void showCameraFrame(cv::Mat& frame);









//    AppInit*    appInit;

//    QTimer*     m_timer = nullptr;
//    int         fps = 30;
//    void onTimer();
//    ImageProcessor processor;

//    FlipHorizontallyHandler* hflip= new FlipHorizontallyHandler;
//    FlipVerticallyHandler* vflip = new FlipVerticallyHandler;
//    RotateHandler* thr = new RotateHandler;
//    BGR2RGBHandler* bgr2rgb = new BGR2RGBHandler;
//    ColorAdjustHandler* cah  = new ColorAdjustHandler;
//private slots:
//    void on_m_btn_open_camera_clicked(bool checked);
//    void showFrame(cv::Mat frame);
//    void readFrame() ;
//    //滑动条
//    void on_exit_action_triggered();
//    void on_pushButton_clicked();
//    void on_action_2_triggered();
//    void on_action_7_triggered();
//    void on_pushButton_5_clicked(bool checked);
//    void on_horizontalSlider_7_valueChanged(int value);
//    void on_checkBox_2_stateChanged(int arg1);
//    void on_checkBox_3_stateChanged(int arg1);
//    void on_pushButton_6_clicked(bool checked);
//    void on_pushButton_7_clicked(bool checked);
//    void on_horizontalSlider_4_valueChanged(int value);
//    void on_horizontalSlider_5_valueChanged(int value);
//    void on_horizontalSlider_6_valueChanged(int value);
//    void on_pushButton_7_clicked();

     void on_m_btn_open_camera_clicked();
     void on_m_cbx_camera_list_currentIndexChanged(int index);
     void on_m_cbx_camera_type_currentIndexChanged(int index);
     void on_m_cbx_camera_type_currentTextChanged(const QString &arg1);
     void on_m_btn_reset_clicked();
     void on_action_normal_triggered();
     void on_action_maxscreen_triggered();
     void on_action_fullscreen_triggered();
     void on_action_exit_triggered();
     void on_action_open_image_triggered();
     void on_action_about_triggered();
};
#endif // MAINWINDOW_H
