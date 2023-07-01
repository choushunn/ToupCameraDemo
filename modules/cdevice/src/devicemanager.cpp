#include "devicemanager.h"
#include "qdebug.h"

/**
 * @brief 获取 DeviceManager 的全局单例对象
 *
 * @return DeviceManager 的全局单例对象
 */
DeviceManager& DeviceManager::getInstance()
{
    // DeviceManager全局单例模式
    static DeviceManager instance;
    return instance;
}

/**
 * @brief 构造函数，创建设备工厂对象
 */
DeviceManager::DeviceManager()
{
    // 创建设备工厂对象
    m_deviceFactory = new DeviceFactory();
}

/**
 * @brief 初始化相机
 *
 * @param camera_type 相机类型
 * @param index 相机索引（可选）
 */
CCamera* DeviceManager::initCamera(std::string camera_type)
{
    // 创建相机对象
    m_camera = m_deviceFactory->createCamera(camera_type);
    return m_camera;
}

/**
 * @brief 初始化串口
 */
void DeviceManager::initSerialPort()
{
    // 创建串口对象
    m_serialPort = m_deviceFactory->createSerialPort();
}

/**
 * @brief 初始化 WebSocket
 */
CWebSocketServer*  DeviceManager::initWebSocketServer()
{
    // 创建WebSocket对象
    m_webSocketServer = m_deviceFactory->createWebSocketServer();
    return m_webSocketServer;
}

/**
 * @brief 获取相机对象
 *
 * @return 相机对象指针
 */
CCamera* DeviceManager::getCamera()
{
    return m_camera;
}

/**
 * @brief 获取串口对象
 *
 * @return 串口对象指针
 */
CSerialPort* DeviceManager::getSerialPort()
{
    return m_serialPort;
}

/**
 * @brief 获取 WebSocket 对象
 *
 * @return WebSocket 对象指针
 */
CWebSocketServer* DeviceManager::getWebSocketServer()
{
    return m_webSocketServer;
}

