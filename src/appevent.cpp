#include "appevent.h"


AppEvent::AppEvent(QObject *parent)
        : QObject{parent} {
    //    全局事件中转处理类 appevent.h 用来中转系统中各种跨多个UI以及多个类的事件。
    //    此类必须是全局单例类，便于全局统一使用。
    //    比如类a的父类是b，类b的父类是c，现在有个信号要发给类d，在没有事件中转处理的情况下的做法是将a信号发给b，b再发给c，c再发给d，如果父类嵌套层级越多越复杂。
    //    将类a的信号发给appevent类，然后类d直接关联appevent类进行处理就行。
    //    项目越大，会越发现事件中转处理的必要性，代码清晰，管理方便。
    /*
     * -----------------------------------------------------------------------
    // 连接信号和槽
    // QObject::connect(ui->pushButton_4, &QPushButton::clicked, &appEvent,
    //                std::bind(&AppEvent::sendEvent,  &appEvent, "clicked", std::placeholders::_1));
    // 注册事件
    // appEvent.registerReceiver(this, "clicked");

    // 取消注册事件
    // AppEvent::instance().unregisterReceiver(this, "clicked");
     * -----------------------------------------------------------------------
     */
}


AppEvent& AppEvent::instance() {
    // 返回全局唯一的AppEvent实例
    static AppEvent instance;
    return instance;
}

void AppEvent::registerReceiver(QObject* receiver, const QString& eventName) {
    QMutexLocker locker(&m_mutex);
    // 将接收器添加到指定事件的接收器列表中
    m_receivers[eventName].append(receiver);
}

void AppEvent::unregisterReceiver(QObject* receiver, const QString& eventName) {
    QMutexLocker locker(&m_mutex);
    // 从指定事件的接收器列表中移除接收器
    m_receivers[eventName].removeAll(receiver);
}

void AppEvent::sendEvent(const QString& eventName, const QVariant& eventData) {
    QMutexLocker locker(&m_mutex);
    // 遍历指定事件的接收器列表，调用接收器的handleEvent方法
    for (QObject* receiver : m_receivers[eventName]) {
        QMetaObject::invokeMethod(receiver, "handleEvent", Qt::QueuedConnection,
                                  Q_ARG(QString, eventName),
                                  Q_ARG(QVariant, eventData));
    }
}
