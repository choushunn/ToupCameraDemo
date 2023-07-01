#ifndef DEVICEMANAGER_H
#define DEVICEMANAGER_H
/*
 * 设备管理类
 * 负责管理系统中的各种设备，包括相机、串口、WebSocket等，提供设备初始化、打开、关闭、读取数据等接口，为其他模块提供设备服务。
 */

#include <ccamera.h>
#include "cserialport.h"
#include "cwebsocketserver.h"

class DeviceFactory
{
public:
    CCamera* createCamera(std::string camera_type)
    {
        // 根据相机类型和索引创建相机对象
        return CCamera::getInstance(camera_type);
    }

    CSerialPort* createSerialPort(QObject* parent = nullptr)
    {
        // 创建串口对象
        return new CSerialPort(parent);
    }

    CWebSocketServer* createWebSocketServer(QObject* parent = nullptr)
    {
        // 创建WebSocket对象
        return new CWebSocketServer(parent);
    }
};

class DeviceManager
{
public:
    static DeviceManager& getInstance();
    CCamera* getCamera();
    CSerialPort *getSerialPort();
    CWebSocketServer* getWebSocketServer();
    // 成员函数声明
    CCamera* initCamera(std::string camera_type);
    void initSerialPort();
    CWebSocketServer*  initWebSocketServer();
private:
    DeviceManager();
    DeviceManager(const DeviceManager&) = delete;
    DeviceManager& operator=(const DeviceManager&) = delete;
    ~DeviceManager(){};

    // 设备对象成员变量
    CCamera* m_camera;
    CSerialPort* m_serialPort;
    CWebSocketServer* m_webSocketServer;
    // 设备工厂成员变量
    DeviceFactory* m_deviceFactory;
};



#endif // DEVICEMANAGER_H
