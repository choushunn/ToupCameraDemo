#ifndef CYOLOX_H
#define CYOLOX_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CYoloX : public COnnx
{
public:
    CYoloX();
    CYoloX(bool isGPU=false);
    ~CYoloX();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;

private:
//    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
//    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
    void getModelInfo();
    fastdeploy::vision::detection::YOLOX*  model;
};

#endif // CYOLOX_H
