#include "devicemanager.h"
#include "qdebug.h"

DeviceManager::DeviceManager(QObject *parent)
    : QObject{parent}
{
    //    initCamera();
}

void DeviceManager::initCamera(std::string camera_type, int index)
{
    m_camera = CCamera::createInstance(camera_type);
}



void DeviceManager::initSerialPort()
{

}

void DeviceManager::initWebSocket()
{

}

void DeviceManager::getCameraList(std::vector<std::string> &camera_list)
{
    // 获取相机列表
    m_camera->getCameraList(camera_list);
}

bool DeviceManager::openCamera(int index)
{
    // 打开相机
    if(m_camera->open(index) == 200){
        return true;
    }
    return false;
}

void DeviceManager::closeCamera()
{
    // 关闭相机
    m_camera->close();
}

bool DeviceManager::cameraOpened()
{
    return m_camera->isOpened();
}

void DeviceManager::readCamera()
{
    cv::Mat frame;
    m_camera->read(frame);
    if (!frame.empty()) {
        emit sendCameraFrame(frame);
        qDebug() << "DeviceManager:readCamereFrame 成功.";
    }
}
