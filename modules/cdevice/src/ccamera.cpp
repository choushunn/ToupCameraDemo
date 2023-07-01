#include "ccamera.h"
#include "ctoupcamera.h"
#include "cusbcamera.h"
#include <QString>


CCamera* CCamera::getInstance(std::string type)
{
    if (type == "USB")
    {
        qDebug() << "CCamera:创建USB相机成功";
//        return CUSBCamera::getInstance();
        return new CUSBCamera();

    }
    else if (type == "TOUP")
    {
        qDebug() << "CCamera:创建TOUP相机成功";
//        return CToupCamera::getInstance();
        return new CToupCamera();
    }
    else
    {
        qDebug() << "CCamera:创建摄像头失败";
        throw std::invalid_argument("Invalid camera type");
        // return nullptr;
    }
}

