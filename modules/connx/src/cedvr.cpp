#include "cedvr.h"

CEDVR::CEDVR()
{
    auto model_file = "models/edvr/model.pdmodel";
    auto params_file = "models/edvr/model.pdiparams";
    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    option.UsePaddleBackend();
    model = new fastdeploy::vision::sr::EDVR(model_file, params_file,option);

    if (!model->Initialized()) {
        std::cerr << "Failed to initialize." << std::endl;
        return;
    }

}

CEDVR::CEDVR(bool isGPU)
{
    auto model_file = "models/edvr/model.pdmodel";
    auto params_file = "models/edvr/model.pdiparams";
    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    option.UsePaddleBackend();
    model = new fastdeploy::vision::sr::EDVR(model_file, params_file, option);

    if (!model->Initialized()) {
        std::cerr << "Failed to initialize." << std::endl;
        return;
    }
}

void CEDVR::run(const cv::Mat input_image, cv::Mat &output_image)
{
    int out_width = 1280;
    int out_height = 720;
    cv::Mat frame;
    if(input_image.empty()){
        return;
    }
    cv::resize(input_image, frame, cv::Size(out_width, out_height));
    std::vector<cv::Mat> imgs{frame,frame};
    std::vector<cv::Mat> results;
    model->Predict(imgs, results);
    output_image = results[0];
}

void CEDVR::preProcessing(const cv::Mat &input_image, Ort::Value &input_tensor)
{

}

void CEDVR::postProcessing(Ort::Value &output_tensor, cv::Mat &output_image)
{

}
