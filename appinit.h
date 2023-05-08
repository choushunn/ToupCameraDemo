#ifndef APPINIT_H
#define APPINIT_H

#include <QMainWindow>
#include <QMediaDevices>
#include <QCamera>
#include "ui_mainwindow.h"
#include "cusbcamera.h"
#include "ctoupcamera.h"
#include "utils.h"

namespace Ui{
class MainWindow;
}

class AppInit : public QMainWindow
{
    Q_OBJECT
public:
    AppInit();
    explicit AppInit(Ui::MainWindow *ui);
    CUSBCamera*     webCamera  = nullptr;
    CToupCamera*    toupCamera = nullptr;




private:
    Ui::MainWindow       *mainwindowUi;
    QList<QCameraDevice> m_cameraList;
    int m_cameraIndex;

private:
    void initMainWindowUI();
    void initCamera();
    void initToupCamera();
};

#endif // APPINIT_H
