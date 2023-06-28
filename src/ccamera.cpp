#include "ccamera.h"
#include "ctoupcamera.h"
#include "cusbcamera.h"

#include <QString>

CCamera::CCamera()
{

}

CCamera* CCamera::createInstance(const std::string type) {

    if (type == "USB") {
        qDebug() << "CCamera:创建USB相机成功"<< QString::fromStdString(type);
        return new CUSBCamera();

    } else if (type == "TOUP") {
        qDebug() << "CCamera:创建TOUP相机成功"<< QString::fromStdString(type);
        return new CToupCamera();

    } else {
        qDebug() << "CCamera:创建摄像头失败"<< QString::fromStdString(type);
        return nullptr;
    }


}
