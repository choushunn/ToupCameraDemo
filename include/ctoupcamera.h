#ifndef CTOUPCAMERA_H
#define CTOUPCAMERA_H

#include <toupcam.h>
#include <QString>
#include "ccamera.h"
#include "qdebug.h"


class CToupCamera : public CCamera
{

public:
    CToupCamera();
    CToupCamera(int index);
    ~CToupCamera();
    bool isOpened() const override;
    int open() override;
    int open(int index) override;
    void close() override;
    bool read(cv::Mat& frame) override;
    void getCameraList(std::vector<std::string> &camera_list) override;
private:
    int m_index;
    HToupcam        m_hcam =nullptr;
    uchar*          m_pData = nullptr;
    int             m_res;
    unsigned        m_imgWidth;
    unsigned        m_imgHeight;
    ToupcamFrameInfoV2* pInfo;
    ToupcamDeviceV2 m_arr[TOUPCAM_MAX]; //所有相机
    unsigned toupCamCount;
private:
    void evtCallback(unsigned nEvent);
    static void __stdcall eventCallBack(unsigned nEvent, void *pCallbackCtx);
};

#endif // CTOUPCAMERA_H
