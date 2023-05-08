#include "appinit.h"


/**
 * @brief AppInit 构造函数
 * @param
 */
AppInit::AppInit()
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
}

/**
 * @brief AppInit 构造函数
 * @param
 */
AppInit::AppInit(Ui::MainWindow *ui)
    :mainwindowUi(ui)
{
    //UI初始化
    initMainWindowUI();
    //摄像头初始化
    initCamera();
    //相机类型切换检测
    connect(mainwindowUi->m_cbx_camera_list, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        m_cameraIndex = index;
        qDebug() << "AppInit:camera index changed:" << m_cameraIndex;
    });

    connect(mainwindowUi->m_cbx_camera_type, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        qDebug() << "AppInit:camera type changed:" << mainwindowUi->m_cbx_camera_type->currentText();
        if(mainwindowUi->m_cbx_camera_type->currentText()=="USB"){
            initCamera();
        }else if(mainwindowUi->m_cbx_camera_type->currentText()=="TOUP"){
            initToupCamera();
        }
        else
        {
            qDebug() <<"AppInit:Camera Failed.";
        }

    });

}

/**
 * @brief 界面初始化
 * @param
 */
void AppInit::initMainWindowUI()
{
    qDebug() << "AppInit:UI初始化完成.";
}

/**
 * @brief USB摄像头初始化
 * @param
 */
void AppInit::initCamera()
{
    mainwindowUi->m_cbx_camera_list->clear();
    //查询可用相机
    m_cameraList = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : m_cameraList) {
        mainwindowUi->m_cbx_camera_list->addItem(cameraDevice.description());
    }

    if(m_cameraList.count()==0){
        mainwindowUi->m_cbx_camera_list->setEditable(true);
        mainwindowUi->m_cbx_camera_list->setCurrentText("未检测到相机");
        mainwindowUi->m_cbx_camera_list->setDisabled(true);
        mainwindowUi->m_btn_open_camera->setDisabled(true);
        qDebug() << "AppInit:USB摄像头初始化失败." << "检测到"<< m_cameraList.count() << "个USB摄像头.";
        return;
    }else{
        mainwindowUi->m_cbx_camera_list->setEditable(false);
        mainwindowUi->m_cbx_camera_list->setDisabled(false);
        mainwindowUi->m_btn_open_camera->setDisabled(false);
        m_cameraIndex = mainwindowUi->m_cbx_camera_list->currentIndex();
        //        appThread = new QThread();
        webCamera = new CUSBCamera(m_cameraIndex);
        //        camera->moveToThread(appThread);
        //        appThread->start();
    }
    qDebug() << "AppInit:USB摄像头初始化完成." << "检测到"<< m_cameraList.count() << "个USB摄像头.";
}

/**
 * @brief Toup摄像头初始化
 * @param
 */
void AppInit::initToupCamera()
{
    mainwindowUi->m_cbx_camera_list->clear();
    // 显示可用相机
    ToupcamDeviceV2 arr[TOUPCAM_MAX];
    unsigned toupCamCount = Toupcam_EnumV2(arr);

    if (0 == toupCamCount){
        mainwindowUi->m_cbx_camera_list->setEditable(true);
        mainwindowUi->m_cbx_camera_list->setCurrentText("未检测到相机");
        mainwindowUi->m_cbx_camera_list->setDisabled(true);
        mainwindowUi->m_btn_open_camera->setDisabled(true);
        qDebug() << "AppInit:Toup摄像头初始化失败." << "检测到"<< 0 << "个Toup摄像头.";
        return;
    }
    else
    {
        mainwindowUi->m_cbx_camera_list->setEditable(false);
        mainwindowUi->m_cbx_camera_list->setDisabled(false);
        mainwindowUi->m_btn_open_camera->setDisabled(false);
        for (unsigned i = 0; i < toupCamCount; ++i)
        {
            //循环每个相机
#ifdef _WIN32
            mainwindowUi->m_cbx_camera_list->addItem(QString::fromWCharArray(arr[i].displayname));
#elif
            mainwindowUi->m_cbx_camera_list->addItem(arr[i].displayname);
#endif
        }
        m_cameraIndex = mainwindowUi->m_cbx_camera_list->currentIndex();
        toupCamera = new CToupCamera(arr[m_cameraIndex]);
    }
    qDebug() << "AppInit:Toup摄像头初始化完成." << "检测到"<< toupCamCount << "个Toup摄像头.";
}






