#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    appInit  = new AppInit(ui);
    appEvent = new AppEvent(this);
    m_timer  = new QTimer();
}

// 创建FrameProcessing视频流处理对象
//    frameProcessing = new FrameProcessing(this);

//连接 MainWindow的sendFrame信号和VideoProcessing的processFrame槽函数
//    connect(this, &MainWindow::sendFrame, frameProcessing, &FrameProcessing::processFrame);



/**
 * @brief 打开相机信号槽
 * @param
 */
void MainWindow::on_m_btn_open_camera_clicked(bool checked)
{
    if(checked){
        ui->m_btn_open_camera->setText("关闭");
        //1s读10帧
        //        fps = ui->lineEdit_FPS->text().toInt();
        m_timer->setInterval(int(1000/fps));
        //        connect(appInit->ncnnYolo, &CNcnn::sendDectectImage, this, &::MainWindow::showFrame);
        if(ui->m_cbx_camera_type->currentText() == "USB"){
            appInit->webCamera->open();
            m_timer->start();
            //读取帧
            connect(m_timer, &QTimer::timeout, appInit->webCamera, &CUSBCamera::read);
            //处理帧
            connect(appInit->webCamera, &CUSBCamera::sendFrame, appEvent, &AppEvent::processFrame);
            //显示帧
            connect(appEvent, &AppEvent::sendProcessFrame, this, &MainWindow::showFrame);
        }
        else if(ui->m_cbx_camera_type->currentText() == "TOUP")
        {
            //打开摄像头
            appInit->toupCamera->open();
            m_timer->start();
            //读取帧
            connect(m_timer, &QTimer::timeout, appInit->toupCamera, &CToupCamera::read);
            //处理帧
            //            connect(appInit->toupCamera, &CToupCamera::sendFrame, appInit->ncnnYolo, &CNcnn::detect);
            //显示帧
            //connect(appInit->toupCamera, &CToupCamera::sendImage, this, &::MainWindow::showFrame);
        }
    }
    else
    {
        if(ui->m_cbx_camera_type->currentText()=="USB"){
            m_timer->stop();
            appInit->webCamera->close();
        }else if(ui->m_cbx_camera_type->currentText()=="TOUP")
        {
            appInit->toupCamera->close();
        }
        ui->m_btn_open_camera->setText("打开");
        ui->m_lbl_display1->clear();
        ui->m_lbl_display2->clear();
    }
}


/**
 * @brief 显示QImage
 * @param image    接收到的QImage
 */
void MainWindow::showFrame(QImage image1, QImage image2)
{
    //    qDebug() << "MainWindow:3.show frame.";
    QImage image11 = image1.scaled(ui->m_lbl_display1->width(), ui->m_lbl_display1->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->m_lbl_display1->setPixmap(QPixmap::fromImage(image11));
    QImage image21 = image2.scaled(ui->m_lbl_display2->width(), ui->m_lbl_display2->height(), Qt::KeepAspectRatio, Qt::FastTransformation);
    ui->m_lbl_display2->setPixmap(QPixmap::fromImage(image21));
}




/**
 * @brief 信号槽，灰度化
 * @param
 */
void MainWindow::on_m_btn_graypro_clicked(bool checked)
{
    qDebug() << ui->m_btn_Bchannel->isChecked();
    if(ui->m_btn_Bchannel->isChecked()){
        ui->m_btn_Bchannel->setChecked(false);
        if(appEvent->m_eventQueue.contains(BEvent)){
            appEvent->m_eventQueue.removeAll((BEvent));
            ui->m_btn_Bchannel->setChecked(false);
        }
        qDebug() << "ischecked";
        return;
    }
    if (checked) {
        if(!appEvent->m_eventQueue.contains(GrayEvent)){
            appEvent->m_eventQueue.append(GrayEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(GrayEvent)){
            appEvent->m_eventQueue.removeAll(GrayEvent);
        }
    }
}


/**
 * @brief 信号槽，水平翻转
 * @param
 */
void MainWindow::on_m_btn_hflip_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(FlipEvent)){
            appEvent->m_eventQueue.append(FlipEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(FlipEvent)){
            appEvent->m_eventQueue.removeAll(FlipEvent);
        }
    }
}

/**
 * @brief 信号槽，灰度直方图
 * @param
 */
void MainWindow::on_m_btn_Hist_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(HistEvent)){
            appEvent->m_eventQueue.append(HistEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(HistEvent)){
            appEvent->m_eventQueue.removeAll(HistEvent);
        }
    }
}


/**
 * @brief 信号槽，rgb直方图
 * @param
 */
void MainWindow::on_m_btn_rgbHist_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(rgbHistEvent)){
            appEvent->m_eventQueue.append(rgbHistEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(rgbHistEvent)){
            appEvent->m_eventQueue.removeAll(rgbHistEvent);
        }
    }
}


/**
 * @brief 信号槽，自适应维纳滤波
 * @param
 */
//void MainWindow::on_m_btn_AWF_clicked(bool checked)
//{
//    if (checked) {
//        if(!appEvent->m_eventQueue.contains(AWFHistEvent)){
//            appEvent->m_eventQueue.append(AWFHistEvent);
//        }
//    } else {
//        if(appEvent->m_eventQueue.contains(AWFHistEvent)){
//            appEvent->m_eventQueue.removeAll(AWFHistEvent);
//        }
//    }
//}

/**
 * @brief 信号槽，HSV转换
 * @param
 */
void MainWindow::on_m_btn_hsvpro_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(HSVEvent)){
            appEvent->m_eventQueue.append(HSVEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(HSVEvent)){
            appEvent->m_eventQueue.removeAll(HSVEvent);
        }
    }
}

/**
 * @brief 信号槽，蓝色通道显示
 * @param
 */
void MainWindow::on_m_btn_Bchannel_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(BEvent)){
            appEvent->m_eventQueue.append(BEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(BEvent)){
            appEvent->m_eventQueue.removeAll(BEvent);
        }
    }
}

/**
 * @brief 信号槽，绿色通道显示
 * @param
 */
void MainWindow::on_m_btn_Gchannel_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(GEvent)){
            appEvent->m_eventQueue.append(GEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(GEvent)){
            appEvent->m_eventQueue.removeAll(GEvent);
        }
    }
}



/**
 * @brief 信号槽，红色通道显示
 * @param
 */
void MainWindow::on_m_btn_Rchannel_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(REvent)){
            appEvent->m_eventQueue.append(REvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(REvent)){
            appEvent->m_eventQueue.removeAll(REvent);
        }
    }
}



/**
 * @brief 信号槽，二值化阈值处理
 * @param
 */
void MainWindow::on_m_btn_binarythre_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(BinthEvent)){
            appEvent->m_eventQueue.append(BinthEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(BinthEvent)){
            appEvent->m_eventQueue.removeAll(BinthEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，反二值化阈值处理
 * @param
 */
void MainWindow::on_m_btn_binarythre_inv_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(BinthinvEvent)){
            appEvent->m_eventQueue.append(BinthinvEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(BinthinvEvent)){
            appEvent->m_eventQueue.removeAll(BinthinvEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，截断阈值化处理
 * @param
 */
void MainWindow::on_m_btn_thtr_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(ThtrEvent)){
            appEvent->m_eventQueue.append(ThtrEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(ThtrEvent)){
            appEvent->m_eventQueue.removeAll(ThtrEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，超阈值零处理
 * @param
 */
void MainWindow::on_m_btn_thtoinv_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(ThtoinvEvent)){
            appEvent->m_eventQueue.append(ThtoinvEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(ThtoinvEvent)){
            appEvent->m_eventQueue.removeAll(ThtoinvEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    appEvent->m_val = value;
}

/**
 * @brief 信号槽，低阈值零处理
 * @param
 */
void MainWindow::on_m_btn_thto_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(ThtoEvent)){
            appEvent->m_eventQueue.append(ThtoEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(ThtoEvent)){
            appEvent->m_eventQueue.removeAll(ThtoEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，自适应阈值处理(二值)
 * @param
 */
void MainWindow::on_m_btn_adTh_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(adThEvent)){
            appEvent->m_eventQueue.append(adThEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(adThEvent)){
            appEvent->m_eventQueue.removeAll(adThEvent);
        }
    }
}



/**
 * @brief 信号槽，均值滤波
 * @param
 */
void MainWindow::on_m_btn_avblur_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(avblEvent)){
            appEvent->m_eventQueue.append(avblEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(avblEvent)){
            appEvent->m_eventQueue.removeAll(avblEvent);
        }
    }
}

/**
 * @brief 信号槽，方框滤波
 * @param
 */
void MainWindow::on_m_btn_bofil_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(bofilEvent)){
            appEvent->m_eventQueue.append(bofilEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(bofilEvent)){
            appEvent->m_eventQueue.removeAll(bofilEvent);
        }
    }
}

/**
 * @brief 信号槽，高斯滤波
 * @param
 */
void MainWindow::on_m_btn_gabl_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(gablEvent)){
            appEvent->m_eventQueue.append(gablEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(gablEvent)){
            appEvent->m_eventQueue.removeAll(gablEvent);
        }
    }
}


/**
 * @brief 信号槽，中值滤波
 * @param
 */
void MainWindow::on_m_btn_mebl_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(meblEvent)){
            appEvent->m_eventQueue.append(meblEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(meblEvent)){
            appEvent->m_eventQueue.removeAll(meblEvent);
        }
    }
}


/**
 * @brief 信号槽，双边滤波
 * @param
 */
void MainWindow::on_m_btn_bifi_clicked(bool checked)
{
    if (checked) {
        if(!appEvent->m_eventQueue.contains(bifiEvent)){
            appEvent->m_eventQueue.append(bifiEvent);
        }
    } else {
        if(appEvent->m_eventQueue.contains(bifiEvent)){
            appEvent->m_eventQueue.removeAll(bifiEvent);
        }
    }
}
void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    appEvent->m_val = value;
}


/**
 * @brief 信号槽，2D卷积
 * @param
 */
void MainWindow::on_m_btn_2Dfi_clicked(bool checked)
{
    appEvent->m_kernel = QString();
    if (checked) {
        if(!appEvent->m_eventQueue.contains(tDfiEvent)){
            appEvent->m_eventQueue.append(tDfiEvent);
        }
//        if(ui->com_juanjisuanzi->text()=="lapulasi"){
            appEvent->m_kernel = "lapulasi";
//        }
    } else {
        if(appEvent->m_eventQueue.contains(tDfiEvent)){
            appEvent->m_eventQueue.removeAll(tDfiEvent);
        }
    }
}



/**
 * @brief 析构函数，结束程序时
 * @param
 */
MainWindow::~MainWindow()
{
    delete ui;
}










