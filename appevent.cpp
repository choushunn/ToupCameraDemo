#include "appevent.h"
#include <iostream>

AppEvent::AppEvent(QObject *parent)
    : QObject{parent}
{
    //    全局事件中转处理类 appevent.h 用来中转系统中各种跨多个UI以及多个类的事件。
    //    此类必须是全局单例类，便于全局统一使用。
    //    比如类a的父类是b，类b的父类是c，现在有个信号要发给类d，在没有事件中转处理的情况下的做法是将a信号发给b，b再发给c，c再发给d，如果父类嵌套层级越多越复杂，代码越难管理。
    //    将类a的信号发给appevent类，然后类d直接关联appevent类进行处理就行。
    //    项目越大，会越发现事件中转处理的必要性，代码清晰，管理方便。
}

/**
 * @brief 处理frame
 * @param frame
 */
void AppEvent::processFrame(cv::Mat frame)
{
    qDebug() << "AppEvent:process frame.";
    // 处理当前事件
    cv::Mat frame2 = frame.clone();

    for(MyEventType eventType:m_eventQueue){
        if (eventType  == FlipEvent) {
            cv::flip(frame2, frame2, 1);
        } else if (eventType  == GrayEvent) {
            cv::cvtColor(frame2, frame2, cv::COLOR_RGB2GRAY);
            //        grayImage();
        } else if (eventType  == HistEvent) {
            drawHist(frame2);
        } else if(eventType  == rgbHistEvent){
            qDebug() <<"rgbHistEvent";
            drawrgbHist(frame2);
        }
//        else if(){

//        }
    }
    emit sendProcessFrame(cvMatToQImage(frame), cvMatToQImage(frame2));
}

/**
 * @brief 灰度直方图
 * @param frame
 */
void AppEvent::drawHist(cv::Mat &src)
{
//    std::cout << std::endl << cv::format(src, cv::Formatter::FMT_C) << std::endl << std::endl;
    cv::Mat image_gray, hist;   //定义，灰度图像, 直方图
    cv::cvtColor(src, image_gray,cv::COLOR_BGR2GRAY);  //灰度化
    //获取图像直方图
    int histsize = 256;
    float ranges[] = { 0,255 };
    const float* histRanges = { ranges };
    //如果不需要使用掩膜，则可以将第四个参数设置为 cv::Mat()，即一个空的 cv::Mat 对象。
    cv::calcHist(&image_gray, 1, 0,cv::Mat(), hist, 1, &histsize, &histRanges, true, false);
    //创建直方图显示图像
    int hist_h = 400;//直方图的图像的高
    int hist_w = 512; //直方图的图像的宽
    int bin_w = hist_w / histsize;//直方图的等级
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));//绘制直方图显示的图像,先绘制一个全黑的图像
    //绘制并显示直方图
    cv::normalize(hist, hist, 0, hist_h, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    for (int i = 1; i < histsize; i++)
    {
        cv::line(histImage,cv::Point((i - 1) * bin_w, hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point((i)*bin_w, hist_h - cvRound(hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
    }
//    cv::imshow("histImage", histImage);
//    cv::waitKey(0);  //暂停，保持图像显示，等待按键结束
}


/**
 * @brief rgb直方图
 * @param frame
 */
void AppEvent::drawrgbHist(cv::Mat &src)
{
    std::vector<cv::Mat> all_channel;
    cv::split(src, all_channel);
    int histsize = 256;
    float ranges[] = { 0,255 };
    const float* histRanges = { ranges };
    cv::Mat b_hist;
    cv::Mat g_hist;
    cv::Mat r_hist;
    cv::calcHist(&all_channel[0], 1, 0,cv::Mat(), b_hist, 1, &histsize, &histRanges, true, false);
    cv::calcHist(&all_channel[1], 1, 0,cv::Mat(), g_hist, 1, &histsize, &histRanges, true, false);
    cv::calcHist(&all_channel[2], 1, 0,cv::Mat(), r_hist, 1, &histsize, &histRanges, true, false);
    int hist_h = 400;//直方图的图像的高
    int hist_w = 512; //直方图的图像的宽
    int bin_w = cvRound((double)hist_w/histsize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(b_hist, b_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    cv::normalize(g_hist, g_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    cv::normalize(r_hist, r_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    for (int i = 1; i < histsize; i++)
    {
        cv::line(histImage, cv::Point(bin_w * (i-1), hist_h - cvRound(b_hist.at<float>(i-1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i-1), hist_h - cvRound(g_hist.at<float>(i-1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i-1), hist_h - cvRound(r_hist.at<float>(i-1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
    }
    //展示到屏幕上
    cv::imshow("histImage", histImage);
    cv::waitKey(0);
}




/**
 * @brief 灰度均衡
 * @param frame
 */
