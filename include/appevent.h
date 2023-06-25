#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <qdebug.h>
#include <qcoreevent.h>
#include "utils.h"
#include <QString.h>

// 自定义事件类型
enum MyEventType {
    FlipEvent = QEvent::User + 1,
    GrayEvent,
    ScaleEvent,
    HistEvent,
    rgbHistEvent,
//    AWFHistEvent,
    HSVEvent,
    BEvent,
    BinthEvent,
    BinthinvEvent,
    ThtrEvent,
    ThtoinvEvent,
    ThtoEvent,
    adThEvent,
    GEvent,
    REvent,
    avblEvent,
    bofilEvent,
    gablEvent,
    meblEvent,
    bifiEvent,
    tDfiEvent,
};

class AppEvent : public QObject
{
    Q_OBJECT



public:
    int m_val=127;
    explicit AppEvent(QObject *parent = nullptr);
    QVector<MyEventType> m_eventQueue;//动态数组容器

    void drawHist(cv::Mat &src);
    void drawrgbHist(cv::Mat &src);
//    double calculate_noise_var(cv::Mat src);
//    void adaptive_wiener_filter(cv::Mat& src);
    cv::Mat displayBchannel(cv::Mat &src);
    cv::Mat displayGchannel(cv::Mat &src);
    cv::Mat displayRchannel(cv::Mat &src);

    QString m_kernel;

//    Qstring kernel;


public slots:
    void processFrame(cv::Mat& frame);

signals:
    void sendProcessFrame(QImage image, QImage image2);
};

#endif // APPEVENT_H
