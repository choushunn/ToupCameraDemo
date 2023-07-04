#ifndef CMSVR_H
#define CMSVR_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CMSVR : public COnnx
{
public:
    CMSVR();
    CMSVR(bool isGPU=false);
    ~CMSVR();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
private:
    fastdeploy::vision::detection::YOLOX*  model;
};

#endif // CMSVR_H
