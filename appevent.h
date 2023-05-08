#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <qdebug.h>
#include "qcoreevent.h"
#include "utils.h"

// 自定义事件类型
enum MyEventType {
    FlipEvent = QEvent::User + 1,
    GrayEvent,
    ScaleEvent,
    HistEvent,
    rgbHistEvent,
};

class AppEvent : public QObject
{
    Q_OBJECT
public:
    explicit AppEvent(QObject *parent = nullptr);
    QVector<MyEventType> m_eventQueue;//动态数组容器

    void drawHist(cv::Mat &src);
    void drawrgbHist(cv::Mat &src);


public slots:
    void processFrame(cv::Mat frame);

signals:
    void sendProcessFrame(QImage image, QImage image2);
};

#endif // APPEVENT_H
