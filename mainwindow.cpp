#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_hcam(nullptr)
    , m_imgWidth(0)
    , m_imgHeight(0)
    , m_pData(nullptr)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //todo:界面初始化
    //todo:算法初始化
    //todo:摄像头分离
    //todo:界面样式优化

    //相机初始化
    checkCamera();
    // 创建FrameProcessing视频流处理对象
    //    frameProcessing = new FrameProcessing(this);

    //连接 MainWindow的sendFrame信号和VideoProcessing的processFrame槽函数
    //    connect(this, &MainWindow::sendFrame, frameProcessing, &FrameProcessing::processFrame);
    ui->m_cbx_auto->setEnabled(false);
    ui->m_slider_expoTime->setEnabled(false);
    ui->m_slider_expoGain->setEnabled(false);
    //自动曝光槽函数
    connect(ui->m_cbx_auto, &QCheckBox::stateChanged, this, [this](bool state)
    {
        if (m_hcam)
        {
            Toupcam_put_AutoExpoEnable(m_hcam, state ? 1 : 0);
            ui->m_slider_expoTime->setEnabled(!state);
            ui->m_slider_expoGain->setEnabled(!state);
        }
    });
    connect(ui->m_slider_expoTime, &QSlider::valueChanged, this, [this](int value)
    {
        if (m_hcam)
        {
            //                   ui->m_lbl_expoTime->setText(QString::number(value));
            if (!ui->m_cbx_auto->isChecked())
                Toupcam_put_ExpoTime(m_hcam, value);
        }
    });

    connect(ui->m_slider_expoGain, &QSlider::valueChanged, this, [this](int value)
    {
        if (m_hcam)
        {
            //                   ui->m_lbl_expoGain->setText(QString::number(value));
            if (!ui->m_cbx_auto->isChecked())
                Toupcam_put_ExpoAGain(m_hcam, value);
        }
    });
    //相机类型切换检测
    connect(ui->m_cbx_cam_type, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        m_CAM_TYPE = ui->m_cbx_cam_type->currentText();
        qDebug() << "m_CAM_TYPE:"<<m_CAM_TYPE;
        this->checkCamera();
    });

    connect(ui->m_cbx_list, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [this](int index)
    {
        m_cam_index = index;
        qDebug() << "m_cam_index:"<<m_cam_index;
    });

    //全局事件响应
    connect(this, &MainWindow::evtCallback, this, [this](unsigned nEvent)
    {
        //todo:
        //热插拔设备检测

        //运行时事件
        /* this run in the UI thread */
        if(m_CAM_TYPE=="TOUP")
        {
            if (m_hcam)
            {
                if (TOUPCAM_EVENT_IMAGE == nEvent)
                {
                    //显示图像
                    handleImageEvent();
                }
                else if (TOUPCAM_EVENT_EXPOSURE == nEvent)
                {
                    handleExpoEvent();
                }
                //
                else if (TOUPCAM_EVENT_TEMPTINT == nEvent)
                {
                    //handleTempTintEvent();
                }

                else if (TOUPCAM_EVENT_STILLIMAGE == nEvent)
                {
                    //handleStillImageEvent();
                }

                else if (TOUPCAM_EVENT_ERROR == nEvent)
                {
                    closeToupCamera();
                    //QMessageBox::warning(this, "Warning", "Generic error.");
                }
                else if (TOUPCAM_EVENT_DISCONNECTED == nEvent)
                {
                    closeToupCamera();
                    //QMessageBox::warning(this, "Warning", "Camera disconnect.");
                }
            }
        }

    });

}



/**
 * @brief 析构函数，结束程序时
 * @param
 */
MainWindow::~MainWindow()
{
    delete ui;
}


/**
 * @brief 检测相机并显示
 * @param
 */
void MainWindow::checkCamera(){
    ui->m_cbx_list->clear();
    if(m_CAM_TYPE == "TOUP")
    {
        qDebug() << "正在检测ToupCam...";
        this->checkToupCamera();
    }
    else if(m_CAM_TYPE == "USB")
    {
        qDebug() << "正在检测USBCam...";
        this->checkUSBCamera();
    }
    else
    {
        qDebug() <<"Error:没有设置CAM_TYPE" << m_CAM_TYPE;
        return;
    }

    //改变按钮状态
    if(ui->m_cbx_list->count() <= 0){
        ui->m_cbx_list->setDisabled(true);
        ui->m_btn_open->setDisabled(true);
        ui->m_cbx_list->addItem("未找到设备");
    }else{
        ui->m_cbx_list->setDisabled(false);
        ui->m_btn_open->setDisabled(false);
        ui->m_btn_open->setText("打开");
    }
}

/**
 * @brief 打开相机
 * @param checked
 */
void MainWindow::startCamera()
{
    if(ui->m_cbx_list->count() <= 0)
    {
        qDebug() << "没有找到相机";
        return;
    }

    if(m_CAM_TYPE=="TOUP")
    {
        m_hcam = Toupcam_Open(m_cur.id);
        if (m_hcam)
        {
            //初始化Toup设置
            Toupcam_get_eSize(m_hcam, (unsigned*)&m_res);
            m_imgWidth = m_cur.model->res[m_res].width;
            m_imgHeight = m_cur.model->res[m_res].height;
            //                   const QSignalBlocker blocker(m_cmb_res);
            //                   m_cmb_res->clear();
            //                   for (unsigned i = 0; i < m_cur.model->preview; ++i)
            //                   {
            //                       m_cmb_res->addItem(QString::asprintf("%u*%u", m_cur.model->res[i].width, m_cur.model->res[i].height));
            //                   }
            //                   m_cmb_res->setCurrentIndex(m_res);
            //                   m_cmb_res->setEnabled(true);

            Toupcam_put_Option(m_hcam, TOUPCAM_OPTION_BYTEORDER, 0); //Qimage use RGB byte order
            Toupcam_put_AutoExpoEnable(m_hcam, 1);

            startToupCamera();
            //        QMessageBox::information(this, "Success", QString::asprintf("Camera %ls Connected.", m_cur.displayname), QMessageBox::Ok);
        }

    }
    else if(m_CAM_TYPE == "USB")
    {
        m_cam_index = ui->m_cbx_list->currentIndex();
        m_cap = new cv::VideoCapture();
        bool ret = m_cap->open(m_cam_index, cv::CAP_ANY);
        if(ret)
        {
            qDebug() << "The Camera is open"<< ui->m_cbx_list->currentIndex();
            //放到其他线程读取
            QtConcurrent::run(&MainWindow::readFrame, this);

        }
        else
        {
            m_cap->release();
            qDebug() << "The Camera is open failed." << ui->m_cbx_list->currentIndex();
            return;
        }
    }
    else
    {
        //        m_camera->start(cam_index)
    }
    //    ui->m_btn_open_camera->setText("关闭");
}

/**
 * @brief 打开Toup相机
 * @param
 */
void MainWindow::startToupCamera(){
    //启动相机
    if (m_pData)
    {
        delete[] m_pData;
        m_pData = nullptr;
    }

    m_pData = new uchar[TDIBWIDTHBYTES(m_imgWidth * 24) * m_imgHeight];
    unsigned uimax = 0, uimin = 0, uidef = 0;
    unsigned short usmax = 0, usmin = 0, usdef = 0;
    Toupcam_get_ExpTimeRange(m_hcam, &uimin, &uimax, &uidef);
    ui->m_slider_expoTime->setRange(uimin, uimax);
    Toupcam_get_ExpoAGainRange(m_hcam, &usmin, &usmax, &usdef);
    ui->m_slider_expoGain->setRange(usmin, usmax);
    handleExpoEvent();
    if (SUCCEEDED(Toupcam_StartPullModeWithCallback(m_hcam, eventCallBack, this)))
    {

        qDebug()<<"ToupCam打开成功";
        ui->m_cbx_auto->setEnabled(true);
        ui->m_slider_expoTime->setEnabled(true);
        ui->m_slider_expoGain->setEnabled(true);

    }
    else
    {
        qDebug()<<"ToupCam打开失败";
        closeToupCamera();
    }
}

/**
 * @brief 检测USB相机
 * @param
 */
void MainWindow::checkUSBCamera()
{
    //查询可用相机
    m_camera_list = QMediaDevices::videoInputs();
    for (const QCameraDevice &cameraDevice : m_camera_list) {
        qDebug() << cameraDevice.description() << cameraDevice.id() << cameraDevice.isDefault();
        ui->m_cbx_list->addItem(cameraDevice.description());
    }
}

/**
 * @brief 选择ToupCam相机
 * 需要监听事件
 * @param
 */
void MainWindow::checkToupCamera()
{
    // 显示可用相机
    ToupcamDeviceV2 arr[TOUPCAM_MAX] = { 0 };
    unsigned count = Toupcam_EnumV2(arr);

    if (0 == count){
        //        QMessageBox::warning(this, "Warning", "No Camera Found.");
        //        ui->m_cbx_list->addItem("No Camera.");
    }
    else
    {
        for (unsigned i = 0; i < count; ++i)
        {
            //循环每个相机
            m_cur = arr[i];
            ui->m_cbx_list->addItem(QString::fromWCharArray(m_cur.displayname));
        }
    }
}




/**
 * @brief 关闭相机
 * @param
 */
void MainWindow::closeCamera()
{
    if(m_CAM_TYPE=="TOUP")
    {
        closeToupCamera();
        qDebug()<<"ToupCam已关闭";
    }
    else if(m_CAM_TYPE=="USB")
    {
        closeUSBCamera();
        qDebug()<<"USBCam已关闭";
    }
    else
    {
        return;
    }
    ui->m_lbl_display1->clear();
    ui->m_lbl_display2->clear();
}

/**
 * @brief 关闭Toup相机
 * @param
 */
void MainWindow::closeToupCamera()
{

    if (m_hcam)
    {
        Toupcam_Close(m_hcam);
        m_hcam = nullptr;
    }
    delete[] m_pData;
    m_pData = nullptr;

    //    m_btn_open->setText("打开相机");
    //    m_btn_pause->setText("暂停");
    //    m_btn_pause->setEnabled(false);
    //    m_btn_snap->setEnabled(false);
    //    m_btn_hflip->setEnabled(false);
    //    m_btn_vflip->setEnabled(false);

    //    m_cmb_res->setEnabled(false);
    //    m_cmb_res->clear();
    //    m_lbl_video->clear();
    //    m_timer->stop();
    //    ui->statusbar->clearMessage();
    //    //    m_lbl_frame->clear();
    ui->m_cbx_auto->setEnabled(false);
        ui->m_slider_expoGain->setEnabled(false);
         ui->m_slider_expoTime->setEnabled(false);
    //    //    m_btn_autoWB->setEnabled(false);
    //    m_cbox_autoWB->setEnabled(false);
    //    m_slider_temp->setEnabled(false);
    //    m_slider_tint->setEnabled(false);
}

/**
 * @brief 关闭USB相机
 * @param
 */
void MainWindow::closeUSBCamera()
{

    if(m_cap->isOpened()){
        m_cap->release();
        qDebug() << "The Camera is closed."<< ui->m_cbx_list->currentIndex();
    }

}

/**
 * @brief 读取视频帧
 * @param
 */
void MainWindow::readFrame(){
    cv::Mat frame;
    while (m_cap->isOpened())
    {
        bool ret = m_cap->read(frame);
        if(ret)
        {
            //检测
            qDebug() << "1->read frame success.";
            showFrame(frame);
            //                        emit sendFrame(frame);
        }
    }
}

/**
 * @brief 显示帧槽函数
 * @param
 */
void MainWindow::showFrame(cv::Mat frame){

    qDebug() << "4->show frame success.";
    //    cv::resize(frame,frame,cv::Size(ui->m_lbl_display->width(),ui->m_lbl_display->height()));

    if(!m_cap->isOpened()){
        ui->m_lbl_display1->setText("相机已关闭");
        ui->m_lbl_display2->setText("相机已关闭");
    }else{
        ui->m_lbl_display2->setPixmap(QPixmap::fromImage(cvMatToQImage(frame)));
        cv::blur(frame, frame,cv::Size(5,5));
        ui->m_lbl_display1->setPixmap(QPixmap::fromImage(cvMatToQImage(frame)));
    }
}

void MainWindow::showFrame(QImage image){

    //Qt Label 控件显示 OpenCV Mat 格式图像
    //    算法优化图像
    ui->m_lbl_display2->setPixmap(QPixmap::fromImage(image));
    //    原始图像
    cv::Mat frame = QImageTocvMat(image);
    cv::blur(frame, frame, cv::Size(3,3));
    image = cvMatToQImage(frame);
    ui->m_lbl_display1->setPixmap(QPixmap::fromImage(image));
}

/**
 * @brief 读取视频帧
 * @param
 */
void MainWindow::readCameraFrame(){
    cv::Mat frame;
    while (m_cap->isOpened())
    {
        //        for(int i=0;i<2;i++){
        //            m_cap->read(frame);
        //        }
        bool ret = m_cap->read(frame);
        if(ret)
        {
            //检测
            qDebug() << "1->read frame success.";
            emit sendFrame(frame);
        }
        else
        {
            //            QThread::msleep(1);           // 防止频繁读取或者读取不到
        }
    }
}


/**
 * @brief 自动信号槽函数
 * @param checked
 */
void MainWindow::on_m_btn_open_clicked(bool checked)
{
    if(checked){
        //尝试打开摄像头
        this->startCamera();
        ui->m_btn_open->setText("关闭");
    }
    else
    {
        //关闭摄像头
        this->closeCamera();
        ui->m_btn_open->setText("打开");
    }
}



/**
 * @brief 自动信号槽函数
 * @param
 */
void MainWindow::on_m_btn_reset_clicked()
{
    ui->m_btn_reset->setText("重置");
    closeCamera();
    ui->m_btn_open->setChecked(false);
    ui->m_btn_open->setText("打开");
}



//事件调用
void MainWindow::eventCallBack(unsigned nEvent, void* pCallbackCtx)
{
    MainWindow* pThis = reinterpret_cast<MainWindow*>(pCallbackCtx);
    emit pThis->evtCallback(nEvent);
}



//显示图像事件
void MainWindow::handleImageEvent()
{
    unsigned width = 0, height = 0;
    if (SUCCEEDED(Toupcam_PullImage(m_hcam, m_pData, 24, &width, &height)))
    {
        //        qDebug() <<width<<"x"<<height;
        QImage image(m_pData, width, height, QImage::Format_RGB888);
        QImage newimage = image.scaled(ui->m_lbl_display1->width(), ui->m_lbl_display1->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
        //        cv::Mat frame = QImage2cvMat(image);
        //        showFrame(frame);
        showFrame(newimage);
        //        QtConcurrent::run(&MainWindow::showFrame, this);
    }
}

//曝光事件
void MainWindow::handleExpoEvent()
{
    unsigned time = 0;
    unsigned short gain = 0;
    Toupcam_get_ExpoTime(m_hcam, &time);
    Toupcam_get_ExpoAGain(m_hcam, &gain);
    {
        const QSignalBlocker blocker(ui->m_slider_expoTime);
        ui->m_slider_expoTime->setValue(int(time));
    }
    {
        const QSignalBlocker blocker(ui->m_slider_expoGain);
        ui->m_slider_expoGain->setValue(int(gain));
    }
    //    ui->m_lbl_expoTime->setText(QString::number(time));
    //    ui->m_lbl_expoGain->setText(QString::number(gain));
}
