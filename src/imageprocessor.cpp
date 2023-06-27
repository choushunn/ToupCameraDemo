#include "imageprocessor.h"

// 图像处理器责任链
ImageProcessor::ImageProcessor() : m_head(nullptr) {}

// 析构函数，释放所有处理器的内存
ImageProcessor::~ImageProcessor() {
    ImageHandler *p = m_head;
    while (p != nullptr) {
        ImageHandler *q = p->getNext(); // 获取下一个处理器的指针
        delete p;   // 释放当前处理器的内存
        p = q;      // 指向下一个处理器
    }
}

// 添加处理器，将其链接到责任链中
void ImageProcessor::addHandler(ImageHandler *handler) {
    if (m_head == nullptr) {
        m_head = handler;   // 如果责任链为空，则将其作为第一个处理器
    } else {
        ImageHandler *p = m_head;
        while (p->getNext() != nullptr) {
            p = p->getNext();   // 找到责任链中最后一个处理器
        }
        p->setNext(handler);    // 将新的处理器链接到最后一个处理器的后面
    }
}

// 移除处理器，将其从责任链中删除
void ImageProcessor::removeHandler(ImageHandler *handler) {
    if (m_head == handler) {
        m_head = handler->getNext();    // 如果要移除的处理器是第一个处理器，则将第二个处理器作为新的第一个处理器
        handler->setNext(nullptr);      // 将移除的处理器的指针设置为空指针
    } else {
        ImageHandler *p = m_head;
        while (p != nullptr && p->getNext() != handler) {
            p = p->getNext();   // 找到要移除的处理器的前一个处理器
        }
        if (p != nullptr) {
            p->setNext(handler->getNext()); // 将要移除的处理器的下一个处理器链接到前一个处理器的后面
            handler->setNext(nullptr);      // 将移除的处理器的指针设置为空指针
        }
    }
}

// 清除所有处理器，释放内存
void ImageProcessor::clearHandlers() {
    ImageHandler *p = m_head;
    while (p != nullptr) {
        ImageHandler *q = p->getNext(); // 获取下一个处理器的指针
        p->setNext(nullptr);    // 将当前处理器的指针设置为空指针
        delete p;   // 释放当前处理器的内存
        p = q;      // 指向下一个处理器
    }
    m_head = nullptr;   // 将责任链的第一个处理器的指针设置为空指针
}

// 处理图像，将其传递给责任链中的每个处理器
void ImageProcessor::process(cv::Mat &image) {
    if (m_head != nullptr) {
        m_head->process(image); // 将图像传递给第一个处理器进行处理
    }
}


// 水平翻转处理器
void FlipHorizontallyHandler::process(cv::Mat &image) {
    flip(image, image, 1);
    ImageHandler::process(image);
}

// 垂直翻转处理器
void FlipVerticallyHandler::process(cv::Mat &image) {
    flip(image, image, 0);
    ImageHandler::process(image);
}

// 旋转处理器
void RotateHandler::process(cv::Mat &image) {
    cv::Point2f center(image.cols / 2.0, image.rows / 2.0);
    cv::Mat rot = getRotationMatrix2D(center, m_angle, 1);
    warpAffine(image, image, rot, image.size());
    ImageHandler::process(image);
}



