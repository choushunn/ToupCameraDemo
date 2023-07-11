#include "crkyolov7.h"

CRKYOLOv7::CRKYOLOv7()
{
    auto option = fastdeploy::RuntimeOption();
    option.UseRKNPU2();

    auto format = fastdeploy::ModelFormat::RKNN;
    std::string  model_file = "./models/yolov7-tiny_tk2_RK3588_i8.rknn";
     model =new
        fastdeploy::vision::detection::RKYOLOV7(model_file, option, format);
}

CRKYOLOv7::CRKYOLOv7(bool isGPU)
{
    auto option = fastdeploy::RuntimeOption();
    option.UseRKNPU2();

    auto format = fastdeploy::ModelFormat::RKNN;
    std::string  model_file = "./models/yolov7-tiny_tk2_RK3588_i8.rknn";
   model =new
        fastdeploy::vision::detection::RKYOLOV7(model_file, option, format);
}

void CRKYOLOv7::run(const cv::Mat input_image, cv::Mat &output_image)
{
   auto im = input_image;
   fastdeploy::vision::DetectionResult res;
   fastdeploy::TimeCounter tc;

   tc.Start();
   if (!model->Predict(im, &res)) {
       std::cerr << "Failed to predict." << std::endl;
       return;
   }
   auto vis_im = fastdeploy::vision::VisDetection(im, res, 0.5);
   tc.End();
   tc.PrintInfo("RKYOLOV5 in RKNN");
   std::cout << res.Str() << std::endl;
   output_image=vis_im;
}

void CRKYOLOv7::preProcessing(const cv::Mat &input_image, Ort::Value &input_tensor)
{

}



void CRKYOLOv7::postProcessing(Ort::Value &output_tensor, cv::Mat &output_image)
{

}
