#pragma once
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaDevices>
#include <QCamera>
#include <QImage>
#include <QMediaCaptureSession>
#include <QtConcurrent>
#include <toupcam.h>
#include "utils.h"

//#define CAM_TYPE "TOUP"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_m_btn_open_clicked(bool checked);
    void on_m_btn_reset_clicked();


private:
    Ui::MainWindow *ui;
    QScopedPointer<QCamera> m_camera;
    QList<QCameraDevice> m_camera_list;
    QMediaCaptureSession m_captureSession;
    VideoCapture *m_cap;
    ToupcamDeviceV2 m_cur;
    HToupcam        m_hcam;
    uchar*          m_pData;
    int             m_res;
    unsigned        m_imgWidth;
    unsigned        m_imgHeight;
    int m_cam_index;
    QString m_CAM_TYPE="TOUP";
    //    char *camera_type="\0";

private:
    void startCamera();
    void startToupCamera();
    void checkCamera();
    void closeUSBCamera();
    void closeCamera();
    void closeToupCamera();
    void readCameraFrame();
    void checkToupCamera();
    void checkUSBCamera();
    void showFrame(cv::Mat frame);
    void showFrame(QImage image);
    void readFrame();
    static void __stdcall eventCallBack(unsigned nEvent, void* pCallbackCtx);
    void handleImageEvent();
    void handleExpoEvent();
signals:
    void sendFrame(cv::Mat frame);
    void evtCallback(unsigned nEvent);
};
#endif // MAINWINDOW_H
