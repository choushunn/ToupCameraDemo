#ifndef MYTHREAD_H
#define MYTHREAD_H

//#include "connx.h"
#include "qdebug.h"
#include <QThread>

class MyThread : public QThread
{
    Q_OBJECT
public:
    explicit MyThread(QObject *parent = nullptr): QThread(parent) {};

    // 定义一个成员变量用于存储输入参数
//    cv::Mat frame;

    // 定义一个成员变量用于存储输出结果
//    cv::Mat output_image;
//    COnnx* onnx = nullptr;
    void run() override {
        // 在新线程中运行 onnx->run 方法
//        onnx->run(frame, outpu't_image);
        qDebug() << "erwerwer";
    }
};

#endif // MYTHREAD_H
