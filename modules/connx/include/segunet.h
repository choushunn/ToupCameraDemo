#ifndef SEGUNET_H
#define SEGUNET_H

#include <connx.h>
#include <fastdeploy/vision.h>

class SegUnet : public COnnx
{
public:
    SegUnet();
    SegUnet(bool isGPU=false);
    ~SegUnet();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
private:
    fastdeploy::vision::segmentation::PaddleSegModel* model;
};

#endif // SEGUNET_H
