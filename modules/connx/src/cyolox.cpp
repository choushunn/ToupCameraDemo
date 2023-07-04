#include "cyolox.h"

CYoloX::CYoloX()
{
    // 默认CPU推理
    std::string  model_file = "./models/yolox_s.onnx";
    model = new fastdeploy::vision::detection::YOLOX(model_file);
}

CYoloX::CYoloX(bool isGPU)
{
    // GPU推理
    std::string  model_file = "./models/yolox_s.onnx";
    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    model = new fastdeploy::vision::detection::YOLOX(model_file,"",option);
}

void CYoloX::run(const cv::Mat input_image, cv::Mat &output_image)
{
    if (!model->Initialized()) {
        std::cerr << "Failed to initialize." << std::endl;
        return;
    }
    cv::Mat im = input_image;
    fastdeploy::vision::DetectionResult res;
    if (!model->Predict(&im, &res)) {
        std::cerr << "Failed to predict." << std::endl;
        return;
    }
    std::cout << res.Str() << std::endl;

    auto vis_im = fastdeploy::vision::VisDetection(im, res);
    output_image = vis_im;
}

void CYoloX:: preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor){

}


void CYoloX::postProcessing(Ort::Value& output_tensor, cv::Mat& output_image){

}

