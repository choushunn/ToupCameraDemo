#include "cmsvsr.h"

CMSVSR::CMSVSR()
{
    auto model_file =  "models/msvsr/msvsr.pdmodel";
    auto params_file = "models/msvsr/msvsr.pdiparams";
    model = new fastdeploy::vision::sr::PPMSVSR(model_file, params_file);
}

CMSVSR::CMSVSR(bool isGPU)
{
    auto model_file =  "models/msvsr/msvsr.pdmodel";
    auto params_file = "models/msvsr/msvsr.pdiparams";
    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    model = new fastdeploy::vision::sr::PPMSVSR(model_file, params_file, option);
}

void CMSVSR::run(const cv::Mat input_image, cv::Mat &output_image)
{
    std::vector<cv::Mat> results;
    std::vector<cv::Mat> imgs;
    imgs.push_back(input_image);
    model->Predict(imgs, results);
    output_image= results.front();
}

void CMSVSR::preProcessing(const cv::Mat &input_image, Ort::Value &input_tensor)
{

}

void CMSVSR::postProcessing(Ort::Value &output_tensor, cv::Mat &output_image)
{

}
