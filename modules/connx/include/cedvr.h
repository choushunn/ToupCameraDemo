#ifndef CEDVR_H
#define CEDVR_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CEDVR : public COnnx
{
public:
    CEDVR();
    CEDVR(bool isGPU=false);
    ~CEDVR();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
private:
    fastdeploy::vision::sr::EDVR*  model;
};

#endif // CEDVR_H
