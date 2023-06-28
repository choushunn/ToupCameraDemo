#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H
#include <opencv2/opencv.hpp>

// 图像处理器抽象类，使用责任链模式实现
class ImageHandler {
public:
    // 构造函数，接受一个指向下一个处理器的指针，默认为nullptr
    ImageHandler(ImageHandler *next = nullptr) : m_next(next) {}
    // 虚析构函数，确保正确释放内存
    virtual ~ImageHandler() {}

    // 处理函数，纯虚函数，需要子类实现具体的处理逻辑
    virtual void process(cv::Mat &image) {
        // 如果存在下一个处理器，就继续处理
        if (m_next != nullptr) {
            m_next->process(image);
        }
    }

    // 获取下一个处理器的指针
    ImageHandler *getNext() const {
        return m_next;
    }
    // 设置下一个处理器的指针
    void setNext(ImageHandler *next) {
        m_next = next;
    }

private:
    // 指向下一个处理器的指针
    ImageHandler *m_next;
};


// 图像处理器责任链
class ImageProcessor {
public:
    // 构造函数，初始化m_head指针为空指针
    ImageProcessor();

    ~ImageProcessor();
    // 添加处理器，将其链接到责任链中
    void addHandler(ImageHandler *handler);
    // 移除处理器，将其从责任链中删除
    void removeHandler(ImageHandler *handler);
    // 清除所有处理器，释放内存
    void clearHandlers();
    // 处理图像，将其传递给责任链中的每个处理器
    void process(cv::Mat &image);

    void setVFlip();
private:
    // 指向责任链头部的指针
    ImageHandler *m_head;
};



// 水平翻转处理器
class FlipHorizontallyHandler : public ImageHandler {
public:
    virtual void process(cv::Mat &image) override;
};


// 垂直翻转处理器
class FlipVerticallyHandler : public ImageHandler {
public:
    virtual void process(cv::Mat &image) override;
};



// 旋转处理器
class RotateHandler : public ImageHandler {
public:
    RotateHandler() : m_angle(0) {}
    virtual void process(cv::Mat &image) override;
    void setRotation(double angle) { m_angle = angle; }
private:
    double m_angle;
};


class ImageHandlerFactory {
public:
    static ImageHandler* createInstance(const std::string type) {
        if(type == "hflip") {
            return new FlipHorizontallyHandler();
        }
        else if(type == "vflip") {
            return new FlipVerticallyHandler();
        }else if(type == "ratation"){
            return new RotateHandler();
        }
        else {
            // 如果传入的type参数不是A或B，则返回nullptr
            return nullptr;
        }
    }

    static void destroyHandler(ImageHandler* handler) {
        if(handler != nullptr) {
            delete handler;
            handler = nullptr;
        }
    }

};


//BGR2RGB
class BGR2RGBHandler : public ImageHandler {
public:
    virtual void process(cv::Mat &image) override {
        //颜色转换
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
        ImageHandler::process(image);
    }
};

//颜色调整
class ColorAdjustHandler : public ImageHandler {
public:
    ColorAdjustHandler() : m_hue(0), m_saturation(1), m_contrast(1) {}

    void setHue(double hue) { m_hue = hue; }
    void setSaturation(double saturation) { m_saturation = saturation; }
    void setContrast(double contrast) { m_contrast = contrast; }

    void process(cv::Mat &image) override {
        cv::cvtColor(image, image, cv::COLOR_BGR2HSV);
        ImageHandler::process(image);  // 调用ImageHandler的process函数
    }

private:
    double m_hue;           // 色调调整值
    double m_saturation;    // 饱和度调整值
    double m_contrast;      // 对比度调整值
};


#endif // IMAGEPROCESSOR_H
