#include "connx.h"
#include "connxnaf.h"
#include "connxfacepaint.h"

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
    } else {
        return nullptr;
    }
}
