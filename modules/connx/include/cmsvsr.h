#ifndef CMSVSR_H
#define CMSVSR_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CMSVSR : public COnnx
{
public:
    CMSVSR();
    CMSVSR(bool isGPU=false);
    ~CMSVSR();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
private:
    fastdeploy::vision::sr::PPMSVSR*  model;
};

#endif // CMSVSR_H
