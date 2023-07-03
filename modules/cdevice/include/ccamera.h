#ifndef CCAMERA_H
#define CCAMERA_H
#include <opencv2/opencv.hpp>

struct Context {
    unsigned uimax =0;
    unsigned uimin =0;
    unsigned uidef =0;
    unsigned short usmax=0;
    unsigned short usmin=0;
    unsigned short usdef=0;
};

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
    virtual void setAutoExpo(int state){};
    virtual void setExpoTime(int value){};
    virtual void setExpoTarget(int value){};
    virtual void setExpoGain(int value){};
    virtual void getContext(Context& ctx){};
    virtual void getResolution(std::vector<std::string>& res){};
    virtual void setResolution(int index){};
    virtual void getSnap(){};
};

#endif // CCAMERA_H
