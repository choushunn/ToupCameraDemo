#pragma once
#ifndef UTILS_H
#define UTILS_H

//#include <qdebug.h>
//#include <QImage>


//OpenCV 核心模块
#include <opencv2/core.hpp>
//OpenCV 图像编码模块
#include <opencv2/imgcodecs.hpp>
//OpenCV 图像处理模块
#include <opencv2/imgproc.hpp>
//OpenCV 视频接口模块
#include <opencv2/videoio.hpp>
//OpenCV 图像视频I/O模块
#include <opencv2/highgui.hpp>


int Normalization(int value) ;

//cv::Mat QImageTocvMat(const QImage &image);
//QImage cvMatToQImage(const cv::Mat& mat);

#endif // UTILS_H
