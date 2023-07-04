#ifndef YOLOV5FACE_H
#define YOLOV5FACE_H

#include <connx.h>
#include <fastdeploy/vision.h>

class CYOLOv5Face : public COnnx
{
public:
    CYOLOv5Face();
    CYOLOv5Face(bool isGPU=false);
    ~CYOLOv5Face();
    void run(const cv::Mat input_image, cv::Mat& output_image) override;
    void preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor) override;
    void postProcessing(Ort::Value& output_tensor, cv::Mat& output_image) override;
private:
    fastdeploy::vision::facedet::YOLOv5Face* model;
};

#endif // YOLOV5FACE_H
