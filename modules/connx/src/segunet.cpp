#include "segunet.h"

SegUnet::SegUnet()
{
    // CPU
    auto model_file =  "models/segunet/model.pdmodel";
    auto params_file = "models/segunet/model.pdiparams";
    auto config_file = "models/segunet/deploy.yaml";

    auto option = fastdeploy::RuntimeOption();
    option.UseCpu();
    model = new fastdeploy::vision::segmentation::PaddleSegModel(
        model_file, params_file, config_file, option);

    if (!model->Initialized()) {
        std::cerr << "Failed to initialize." << std::endl;
        return;
    }
}

SegUnet::SegUnet(bool isGPU)
{
    // GPU
    auto model_file =  "models/segunet/model.pdmodel";
    auto params_file = "models/segunet/model.pdiparams";
    auto config_file = "models/segunet/deploy.yaml";

    auto option = fastdeploy::RuntimeOption();
    option.UseGpu();
    model = new fastdeploy::vision::segmentation::PaddleSegModel(
        model_file, params_file, config_file, option);

    if (!model->Initialized()) {
        std::cerr << "Failed to initialize." << std::endl;
        return;
    }
}

void SegUnet::run(const cv::Mat input_image, cv::Mat &output_image)
{
    // 推理
    auto im = input_image;
    fastdeploy::vision::SegmentationResult res;
    if (!model->Predict(&im, &res)) {
        std::cerr << "Failed to predict." << std::endl;
        return;
    }

    std::cout << res.Str() << std::endl;
    auto vis_im = fastdeploy::vision::VisSegmentation(im, res, 0.5);
    output_image = vis_im;
    std::cout << "Visualized result saved in ./vis_result.jpg" << std::endl;

}


void SegUnet:: preProcessing(const cv::Mat& input_image, Ort::Value& input_tensor){

}


void SegUnet::postProcessing(Ort::Value& output_tensor, cv::Mat& output_image){

}
