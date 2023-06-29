#include "appinit.h"
#include <QTranslator>
#include <QStyleFactory>
#include <QApplication>
/**
 * @brief AppInit 构造函数
 * @param
 */
AppInit::AppInit(QObject* parent)
: QObject(parent)
{
    //    读取配置文件。
    //    设置全局字体。
    //    设置全局样式表，建议先读取通用的样式表，然后将额外的样式表内容加到后面一起设置。
    //    设置项目编码。
    //    设置翻译文件，可以加载多个，包括qt内置的qt_zh_CN.qm，用户自己的翻译文件等。
    //    初始化随机数种子。
    //    新建项目中需要的目录，防止没有目录无法保存文件到目录。
    //    初始化数据库，包括打开数据库，载入基础数据比如用户表、设备表等。
    //    启动日志输出类用来启动日志服务。
    //    启动运行时间记录类用来记录每次软件运行开始时间和结束时间。
    //    关联全局事件过滤器处理自定义无边框UI拖动、全局按键处理等。

        setGlobalFont();
        setGlobalStyleSheet();
    //    setProjectEncoding();
    //    setTranslations();
    //    initializeRandomSeed();
    //    createProjectDirectories();
    //    initializeDatabase();
    //    startLoggingService();
    //    startRuntimeRecord();
    //    installEventFilters();
}


AppInit::~AppInit()
{
}

void AppInit::readConfigFile()
{
    // 读取配置文件
}

void AppInit::setGlobalFont()
{
    // 设置全局字体
}

void AppInit::setGlobalStyleSheet()
{
    // 设置全局样式表

    qDebug() << "AppInit:全局样式已设置";
}

void AppInit::setProjectEncoding()
{
    // 设置项目编码
}

void AppInit::setTranslations()
{
    // 设置翻译文件
    //    QTranslator* translator = new QTranslator(qApp);
    //    translator->load(":/translations/qt_zh_CN.qm");
    //    qApp->installTranslator(translator);
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "ToupCameraDemo_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            qApp->installTranslator(&translator);
            break;
        }
    }
    qDebug() << "AppInit:翻译文件已设置";
}


void AppInit::initializeRandomSeed()
{
    // 初始化随机数种子
//    qsrand(QDateTime::currentMSecsSinceEpoch());
}


void AppInit::createProjectDirectories()
{
    // 新建项目中需要的目录
    QDir().mkdir("data");
    QDir().mkdir("logs");
}

void AppInit::initializeDatabase()
{
    // 初始化数据库
}

void AppInit::startLoggingService()
{
    // 启动日志输出类用来启动日志服务
    //    LoggingService::instance().start();
}

void AppInit::startRuntimeRecord()
{
    // 启动运行时间记录类用来记录每次软件运行开始时间和结束时间
    //    RuntimeRecord::instance().start();
}

void AppInit::installEventFilters()
{
    // 关联全局事件过滤器处理自定义无边框UI拖动、全局按键处理等
    //    qApp->installEventFilter(new GlobalEventFilter(qApp));
}


///**
// * @brief findCameraIndex 根据ID查询/dev/video的序号
// * @param cameraId 读取的相机id
// */
//int findCameraIndex(QByteArray cameraId){
//    std::string dev_name_str(cameraId.data(), cameraId.size());
//    std::size_t pos = dev_name_str.find("/dev/video");
//    int dev_num =0;
//    if (pos != std::string::npos) {
//        std::string num_str = dev_name_str.substr(pos + 10);
//        dev_num = std::stoi(num_str);
//        qDebug() << "Device number: " << dev_num;
//        return dev_num;
//    }
//    return 0;
//}


///**
// * @brief 界面初始化
// * @param
// */
//void AppInit::initMainWindowUI()
//{
//    qDebug() << "AppInit:UI初始化完成.";
//}


///**
// * @brief 获取可用相机列表
// * @param
// */
//void getCameraList(std::vector<std::string> &camera_list,std::string& camera_type) {
//    if(camera_type=="TOUP"){
//        ToupcamDeviceV2 m_arr[TOUPCAM_MAX]; //所有相机
//        int toupCamCount = Toupcam_EnumV2(m_arr);
//        for (int i = 0; i < toupCamCount; ++i) {
//            qDebug() << m_arr[i].id << m_arr[i].displayname;
//#ifdef _WIN32
//            camera_list.push_back(QString::fromWCharArray(m_arr[i].displayname).toStdString());
//#else
//            camera_list.push_back(m_arr[i].displayname);
//#endif
//        }
//    }else{
//        //查询可用相机
//        QList<QCameraDevice> cameraList = QMediaDevices::videoInputs();
//        for (const QCameraDevice &cameraDevice : cameraList) {
//            camera_list.push_back(cameraDevice.description().toStdString());
//        }
//    }
//}


///**
// * @brief 摄像头初始化
// * @param
// */
//void AppInit::initCamera()
//{
//    //相机类型切换检测
//    connect(mainwindowUi->m_cbx_camera_list,
//            QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, [this](int index)
//            {
//#ifdef _WIN32
//                m_cameraIndex = index;
//#else
//            if(mainwindowUi->m_cbx_camera_type->currentText() == "USB"){
//                QByteArray dev_name = m_cameraList[index].id();
//                m_cameraIndex = findCameraIndex(dev_name);

//            }else{
//                m_cameraIndex = index;
//            }
//#endif
//                camera = CCamera::createInstance(
//                    camera_type,
//                    m_cameraIndex);
//                qDebug() << "AppInit:camera index changed:" << m_cameraIndex;
//            });

//    connect(mainwindowUi->m_cbx_camera_type,
//            QOverload<int>::of(&QComboBox::currentIndexChanged),
//            this, [this](int index)            {
//                qDebug() << "AppInit:camera type changed:" << mainwindowUi->m_cbx_camera_type->currentText();
//                camera_type = mainwindowUi->m_cbx_camera_type->currentText().toStdString();
//                initCamera();
//            });


//    mainwindowUi->m_cbx_camera_list->clear();
//    camera_type = mainwindowUi->m_cbx_camera_type->currentText().toStdString();

//    // 读取相机列表
//    std::vector <std::string> camera_list;
//    getCameraList(camera_list, camera_type);


//    if(camera_list.empty()){
//        mainwindowUi->m_cbx_camera_list->setEditable(true);
//        mainwindowUi->m_cbx_camera_list->setCurrentText("未检测到相机");
//        mainwindowUi->m_cbx_camera_list->setDisabled(true);
//        mainwindowUi->m_btn_open_camera->setDisabled(true);

//        qDebug() << "AppInit:相机初始化失败." << "检测到"<< 0 << "个相机";
//        return;
//    }else{

//        mainwindowUi->m_cbx_camera_list->setEditable(false);
//        mainwindowUi->m_cbx_camera_list->setDisabled(false);
//        mainwindowUi->m_btn_open_camera->setDisabled(false);

//        for (const std::string &camera: camera_list) {
//            mainwindowUi->m_cbx_camera_list->addItem(camera.c_str());
//        }


//        if(camera_type=="USB"){
//#ifdef _WIN32
//            m_cameraIndex = mainwindowUi->m_cbx_camera_list->currentIndex();
//#else
//            QByteArray dev_name = m_cameraList[m_cameraIndex].id();
//            m_cameraIndex = findCameraIndex(dev_name);
//#endif
//        }else{

//            m_cameraIndex = mainwindowUi->m_cbx_camera_list->currentIndex();
//        }


//        // 加载相机
//        camera = CCamera::createInstance(camera_type, m_cameraIndex);
//        qDebug() << "AppInit:相机初始化完成." << "检测到"<< camera_list.capacity() << "个摄像头.";


//        if(camera == nullptr){
//            qDebug() << "AppInit:相机初始化失败";
//        }
//    }
//}




