#include "cyolov5face.h"

CYOLOv5Face::CYOLOv5Face()
{
    std::string  model_file = "./models/yolov5s-face.onnx";
    model = new fastdeploy::vision::facedet::YOLOv5Face(model_file);
}

CYOLOv5Face::CYOLOv5Face(bool isGPU)
{
    // GPU推理
    std::string  model_file = "./models/yolov5s-face.onnx";

    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    model = new fastdeploy::vision::facedet::YOLOv5Face(model_file,"",option);
}

void CYOLOv5Face::run(const cv::Mat input_image, cv::Mat &output_image)
{
    cv::Mat im = input_image;
    fastdeploy::vision::FaceDetectionResult res;
    if (!model->Predict(&im, &res)) {
        std::cerr << "Failed to predict." << std::endl;
        return;
    }
    std::cout << res.Str() << std::endl;
    auto vis_im = fastdeploy::vision::VisFaceDetection(im, res);
    output_image=vis_im;
}

void CYOLOv5Face::preProcessing(const cv::Mat &input_image, Ort::Value &input_tensor)
{

}

void CYOLOv5Face::postProcessing(Ort::Value &output_tensor, cv::Mat &output_image)
{

}
