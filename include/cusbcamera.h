#ifndef CUSBCAMERA_H
#define CUSBCAMERA_H

#include "ccamera.h"
#include "QDebug"

class CUSBCamera : public CCamera
{
public:
//    static CUSBCamera& getInstance();
    CUSBCamera();
    ~CUSBCamera();
    bool isOpened() const override;
    int open() override ;
    int open(int index) override;
    void close() override;
    bool read(cv::Mat& frame) override;
    void getCameraList(std::vector<std::string> &camera_list) override;
    void saveImage() override;
private:
//    CUSBCamera();
//    CUSBCamera(const CUSBCamera&) = delete;
//    CUSBCamera& operator=(const CUSBCamera&) = delete;
//    ~CUSBCamera() = default;

    int m_index = 0;
    cv::VideoCapture m_capture;
};

#endif // CUSBCAMERA_H
