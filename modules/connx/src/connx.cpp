#include "connx.h"
#include "connxnaf.h"
#include "connxfacepaint.h"
#include "cyolox.h"
#include "segunet.h"
#include "QDebug"
#include "cyolov5face.h"
#include "cedvr.h"
#include "cpicodet.h"
#include "crkyolov7.h"
COnnx::COnnx()
{

}

COnnx* COnnx::createInstance(const std::string& type, bool isGPU) {
    if (type == "NAF") {
        qDebug() << "COnnx:创建NAF模型";
        return new COnnxNAF(isGPU);
    } else if (type == "FP") {
         qDebug() << "COnnx:创建FP模型";
        return new COnnxFacePaint(isGPU);
    } else if(type=="YoloX"){
        qDebug() << "COnnx:创建YoloX模型";
        return new CYoloX(isGPU);
    }else if(type=="SegUnet"){
        qDebug() << "COnnx:创建SegUnet模型";
        return new SegUnet(isGPU);
    }else if(type=="YOLOv5Face"){
        qDebug() << "COnnx:创建YOLOv5Face模型";
        return new CYOLOv5Face(isGPU);
    }else if(type=="EDVR"){
        qDebug() << "COnnx:创建EDVR模型";
        return new CEDVR(isGPU);
    } else if(type=="PicoDet-RK3588"){
        qDebug() << "CPicoDet:创建CPicoDet模型";
        return new CRKYOLOv7(isGPU);
    }else {
        return nullptr;
    }
}
