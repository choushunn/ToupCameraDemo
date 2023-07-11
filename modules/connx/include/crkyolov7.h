#ifndef RKYOLOV7_H
#define RKYOLOV7_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CRKYOLOv7 : public COnnx
{
public:
    CRKYOLOv7();
    CRKYOLOv7(bool isGPU=false);
    ~CRKYOLOv7();
    void run(const cv::Mat input_image, cv::Mat &output_image) override;
private:
     void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
     void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
     fastdeploy::vision::detection::RKYOLOV7 *model;

};

#endif // RKYOLOV7_H
