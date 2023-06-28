#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H
/*
 * 设备管理类
 * 负责管理系统中的各种设备，包括相机、串口、WebSocket等，提供设备初始化、打开、关闭、读取数据等接口，为其他模块提供设备服务。
 */

#include <QObject>
#include <ccamera.h>
//#include <QSerialPort>
//#include <QWebSocket>

class DeviceManager : public QObject
{
    Q_OBJECT
public:
    explicit DeviceManager(QObject *parent = nullptr);
    ~DeviceManager(){};

    void initCamera(std::string camera_type, int index);
    void getCameraList(std::vector<std::string> &camera_list);
    bool openCamera(int index);
    void closeCamera();
    bool cameraOpened();
    void readCamera();
    void initSerialPort();
    void initWebSocket();

    void openSerialPort();
    void openWebSocket();


    void closeSerialPort();
    void closeWebSocket();

    QByteArray readSerialPortData();



signals:
    void sendCameraFrame(cv::Mat& frame);
    // void cameraOpened();
    void cameraClosed();
    void serialPortOpened();
    void serialPortClosed();
    void webSocketOpened();
    void webSocketClosed();

private:
    CCamera* m_camera = nullptr;
    //    QSerialPort* m_serialPort;
    //    QWebSocket* m_webSocket;

};

#endif // DEVICEMANAGER_H
