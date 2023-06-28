#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <QVariant.h>
#include <QMutex>
#include <qdebug.h>

class AppEvent : public QObject
{
    Q_OBJECT
public:
    static AppEvent& instance();
    void registerReceiver(QObject* receiver, const QString& eventName);
    void unregisterReceiver(QObject* receiver, const QString& eventName);
    void sendEvent(const QString& eventName, const QVariant& eventData = QVariant());

private:
    explicit AppEvent(QObject* parent = nullptr);
    Q_DISABLE_COPY(AppEvent)

    QMap<QString, QList<QObject*>> m_receivers;
    QMutex m_mutex;
};

#endif // APPEVENT_H
