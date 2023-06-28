#ifndef CUSBCAMERA_H
#define CUSBCAMERA_H

#include "ccamera.h"
#include "QDebug"

class CUSBCamera : public CCamera
{
public:
    CUSBCamera();
    CUSBCamera(int index);
    ~CUSBCamera();
    bool isOpened() const override;
    int open() override ;
    int open(int index) override;
    void close() override;
    bool read(cv::Mat& frame) override;
    void getCameraList(std::vector<std::string> &camera_list) override;
private:
    int m_index;
    cv::VideoCapture m_capture;
};

#endif // CUSBCAMERA_H
