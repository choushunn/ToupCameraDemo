#ifndef CPICODET_H
#define CPICODET_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CPicoDet : public COnnx
{
public:
    CPicoDet();
    CPicoDet(bool isGPU=false);
    ~CPicoDet();
    void run(const cv::Mat input_image, cv::Mat &output_image) override;
private:
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
    void getModelInfo();
    fastdeploy::vision::detection::PicoDet*  model;
};

#endif // CPICODET_H
