#ifndef APPINIT_H
#define APPINIT_H
/*
 * 设备管理类
 * 负责管理系统中的各种设备，包括相机、串口、WebSocket等，提供设备初始化、打开、关闭、读取数据等接口，为其他模块提供设备服务。
 */
#include <QObject>
#include <QDir>

namespace Ui{
class MainWindow;
}

class AppInit: public QObject
{
    Q_OBJECT

public:
    explicit AppInit(QObject* parent = nullptr);
    ~AppInit();

private:
    void readConfigFile();
    void setGlobalFont();
    void setGlobalStyleSheet();
    void setProjectEncoding();
    void setTranslations();
    void initializeRandomSeed();
    void createProjectDirectories();
    void initializeDatabase();
    void startLoggingService();
    void startRuntimeRecord();
    void installEventFilters();
};

#endif // APPINIT_H
