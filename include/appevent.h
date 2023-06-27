#ifndef APPEVENT_H
#define APPEVENT_H

#include <QObject>
#include <qdebug.h>
#include "utils.h"
#include <QString.h>

class AppEvent : public QObject
{
    Q_OBJECT

public:
    explicit AppEvent(QObject *parent = nullptr);

};

#endif // APPEVENT_H
