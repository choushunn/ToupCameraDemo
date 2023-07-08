#include "cpicodet.h"

CPicoDet::CPicoDet()
{
    std::string model_file = "./models/picodet/model.pdmodel";
    std::string params_file = "./models/picodet/model.pdiparams";
    std::string config_file = "./models/picodet/infer_cfg.yml";
    model = new fastdeploy::vision::detection::PicoDet(model_file, params_file,
                                                        config_file);
}

CPicoDet::CPicoDet(bool isGPU)
{
    std::string model_file = "./models/picodet/model.pdmodel";
    std::string params_file = "./models/picodet/model.pdiparams";
    std::string config_file = "./models/picodet/infer_cfg.yml";
    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    model = new fastdeploy::vision::detection::PicoDet(model_file, params_file,
                                                       config_file, option);
}

void CPicoDet::run(const cv::Mat input_image, cv::Mat &output_image)
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
    auto vis_im = fastdeploy::vision::VisDetection(im, res, 0.5);
    output_image = vis_im;
}

void CPicoDet::preProcessing(const cv::Mat &input_image, Ort::Value &input_tensor)
{

}

void CPicoDet::postProcessing(Ort::Value &output_tensor, cv::Mat &output_image)
{

}
