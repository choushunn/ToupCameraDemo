#ifndef CCAMERA_H
#define CCAMERA_H
#include <opencv2/opencv.hpp>


class CCamera
{
public:
    static CCamera* getInstance(std::string type);
    virtual ~CCamera() = default;
    virtual bool isOpened() const = 0;
    virtual int open() = 0;
    virtual int open(int index) = 0;
    virtual void close() = 0;
    virtual bool read(cv::Mat& frame) = 0;
    virtual void getCameraList(std::vector<std::string>& camera_list) = 0;
    virtual void saveImage() = 0;
};

#endif // CCAMERA_H
