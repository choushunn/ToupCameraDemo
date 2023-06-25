#include "appevent.h"


AppEvent::AppEvent(QObject *parent)
        : QObject{parent} {
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
void AppEvent::processFrame(cv::Mat& frame) {
    qDebug() << "AppEvent:process frame.";
    // 处理当前事件    

    for (MyEventType eventType: m_eventQueue) {
        if (eventType == FlipEvent) {
            cv::flip(frame, frame, 1);
        } else if (eventType == GrayEvent) {
            qDebug() << frame.channels();
            if (frame.channels() == 1) {
                qDebug() << 111;
            }
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
            //        grayImage();
        } else if (eventType == HistEvent) {
            drawHist(frame);
        } else if (eventType == rgbHistEvent) {
            drawrgbHist(frame);
        }
//        else if(eventType  == AWFHistEvent){
//            adaptive_wiener_filter(frame2);
//        }
        else if (eventType == HSVEvent) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2HSV);

        } else if (eventType == BEvent) {
            frame = displayBchannel(frame);
        } else if (eventType == GEvent) {
            frame = displayGchannel(frame);
        } else if (eventType == REvent) {
            frame = displayRchannel(frame);
        } else if (eventType == BinthEvent) {
            cv::threshold(frame, frame, m_val, 255, cv::THRESH_BINARY);
        } else if (eventType == BinthinvEvent) {
            cv::threshold(frame, frame, m_val, 255, cv::THRESH_BINARY_INV);
        } else if (eventType == ThtrEvent) {
            cv::threshold(frame, frame, m_val, 255, cv::THRESH_TRUNC);
        } else if (eventType == ThtoinvEvent) {
            cv::threshold(frame, frame, m_val, 255, cv::THRESH_TOZERO_INV);
        } else if (eventType == ThtoEvent) {
            cv::threshold(frame, frame, m_val, 255, cv::THRESH_TOZERO);
        } else if (eventType == adThEvent) {
            cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
            cv::adaptiveThreshold(frame, frame, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 5, 3);
        } else if (eventType == avblEvent) {
            cv::blur(frame, frame, cv::Size(5, 5));
        } else if (eventType == bofilEvent) {
            cv::boxFilter(frame, frame, -1, cv::Size(5, 5));
        } else if (eventType == gablEvent) {
            cv::GaussianBlur(frame, frame, cv::Size(5, 5), 0, 0);
        } else if (eventType == meblEvent) {
            cv::medianBlur(frame, frame, 5);
        } else if (eventType == bifiEvent) {
            //双边滤波src.data != dst.data  输入图像和输出图像的数据指针不能相同，否则会破坏原始数据
            cv::bilateralFilter(frame, frame, 9, m_val, m_val);
        } else if (eventType == tDfiEvent) {

            cv::Mat kernel;
//            cv::Mat kernel = (Mat_<char>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
            if (m_kernel == "lapulasi") {
                kernel = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);//拉普拉斯 算子
            } else {
                kernel = (cv::Mat_<int>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
            }
            cv::filter2D(frame, frame, CV_8UC3, kernel);
        }
    }
//    emit sendProcessFrame(cvMatToQImage(frame), cvMatToQImage(frame2));
}

/**
 * @brief 灰度直方图
 * @param frame
 */
void AppEvent::drawHist(cv::Mat &src) {
//    std::cout << std::endl << cv::format(src, cv::Formatter::FMT_C) << std::endl << std::endl;
    cv::Mat image_gray, hist;   //定义，灰度图像, 直方图
    cv::cvtColor(src, image_gray, cv::COLOR_BGR2GRAY);  //灰度化
    //获取图像直方图
    int histsize = 256;
    float ranges[] = {0, 255};
    const float *histRanges = {ranges};
    //如果不需要使用掩膜，则可以将第四个参数设置为 cv::Mat()，即一个空的 cv::Mat 对象。
    cv::calcHist(&image_gray, 1, 0, cv::Mat(), hist, 1, &histsize, &histRanges, true, false);
    //创建直方图显示图像
    int hist_h = 400;//直方图的图像的高
    int hist_w = 512; //直方图的图像的宽
    int bin_w = hist_w / histsize;//直方图的等级
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));//绘制直方图显示的图像,先绘制一个全黑的图像
    //绘制并显示直方图
    cv::normalize(hist, hist, 0, hist_h, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    for (int i = 1; i < histsize; i++) {
        cv::line(histImage, cv::Point((i - 1) * bin_w, hist_h - cvRound(hist.at<float>(i - 1))),
                 cv::Point((i) * bin_w, hist_h - cvRound(hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
    }
    cv::imshow("histImage", histImage);
    cv::waitKey(0);  //暂停，保持图像显示，等待按键结束
}


/**
 * @brief r·图
 * @param frame
 */
void AppEvent::drawrgbHist(cv::Mat &src) {
    std::vector <cv::Mat> all_channel;
    cv::split(src, all_channel);
    int histsize = 256;
    float ranges[] = {0, 255};
    const float *histRanges = {ranges};
    cv::Mat b_hist;
    cv::Mat g_hist;
    cv::Mat r_hist;
    cv::calcHist(&all_channel[0], 1, 0, cv::Mat(), b_hist, 1, &histsize, &histRanges, true, false);
    cv::calcHist(&all_channel[1], 1, 0, cv::Mat(), g_hist, 1, &histsize, &histRanges, true, false);
    cv::calcHist(&all_channel[2], 1, 0, cv::Mat(), r_hist, 1, &histsize, &histRanges, true, false);
    int hist_h = 400;//直方图的图像的高
    int hist_w = 512; //直方图的图像的宽
    int bin_w = cvRound((double) hist_w / histsize);
    cv::Mat histImage(hist_h, hist_w, CV_8UC3, cv::Scalar(0, 0, 0));
    cv::normalize(b_hist, b_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    cv::normalize(g_hist, g_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    cv::normalize(r_hist, r_hist, 0, 255, cv::NORM_MINMAX, -1, cv::Mat());//归一化直方图
    for (int i = 1; i < histsize; i++) {
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))), cv::Scalar(0, 0, 255), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))), cv::Scalar(0, 255, 0), 2, 8, 0);
        cv::line(histImage, cv::Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))), cv::Scalar(255, 0, 0), 2, 8, 0);
    }
    //展示到屏幕上
    cv::imshow("histImage", histImage);
    cv::waitKey(0);
}

/**
 * @brief 自适应维纳滤波,噪声方差
 * @param frame
 */
//double AppEvent::calculate_noise_var(cv::Mat src)
//{
//    cv::Mat src_gray;
//    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);

//    cv::Scalar mean, stddev;
//    cv::meanStdDev(src_gray, mean, stddev);

//    double noise_var = stddev.val[0] * stddev.val[0];

//    return noise_var;
//}

//void AppEvent::adaptive_wiener_filter(cv::Mat &src)
//{
//    int block_size = 5;
//    double noise_var = calculate_noise_var(src);
//    cv::Mat src_gray;
//    cv::cvtColor(src, src_gray, cv::COLOR_BGR2GRAY);
//    cv::Mat dst(src_gray.size(), CV_8UC1);
//    int border = block_size / 2;
//    cv::copyMakeBorder(src_gray,src_gray,0,0,480,640,cv::BORDER_REFLECT);
//    for (int i = border; i < src_gray.rows + border; i++)
//    {
//        for (int j = border; j < src_gray.cols + border; j++)
//        {
//            cv::Mat block = src_gray(cv::Rect(j - border, i - border, block_size, block_size));

//            double mean = 0.0;
//            double var = 0.0;

//            for (int k = 0; k < block_size; k++)
//            {
//                for (int l = 0; l < block_size; l++)
//                {
//                    mean += block.at<uchar>(k, l);
//                    var += block.at<uchar>(k, l) * block.at<uchar>(k, l);
//                }
//            }

//            mean /= block_size * block_size;
//            var = var / (block_size * block_size) - mean * mean;

//            double k = var / (var + noise_var);
//            double b = mean - k * mean;

//            double pixel = src_gray.at<uchar>(i, j);

//            dst.at<uchar>(i - border, j - border) = cv::saturate_cast<uchar>(k * pixel + b);
//        }
//    }

//    cv::imshow("Filtered Image", dst);
//    cv::waitKey(0);

//    return dst;
//}

/**
 * @brief 蓝色通道
 * @param frame
 */
cv::Mat AppEvent::displayBchannel(cv::Mat &src) {
    qDebug() << "333";

    std::vector <cv::Mat> all_channel;
    cv::split(src, all_channel);
    cv::Mat frame2 = all_channel[0];
    qDebug() << frame2.channels();
    return frame2;
//    cv::imshow("Blue", all_channel[0]);
//    cv::imshow(
//    cv::waitKey(0);
}

/**
 * @brief 红色通道
 * @param frame
 */
cv::Mat AppEvent::displayGchannel(cv::Mat &src) {

    std::vector <cv::Mat> all_channel;
    cv::split(src, all_channel);
    cv::Mat frame2 = all_channel[1];
    qDebug() << frame2.channels();
    return frame2;
    //    cv::imshow("Blue", all_channel[0]);
    //    cv::imshow(
    //    cv::waitKey(0);
}


/**
 * @brief 绿色通道
 * @param frame
 */
cv::Mat AppEvent::displayRchannel(cv::Mat &src) {

    std::vector <cv::Mat> all_channel;
    cv::split(src, all_channel);
    cv::Mat frame2 = all_channel[2];
    qDebug() << frame2.channels();
    return frame2;
    //    cv::imshow("Blue", all_channel[0]);
    //    cv::imshow(
    //    cv::waitKey(0);
}
