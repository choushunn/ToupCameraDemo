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
void MainWindow::on_gray_process_clicked(bool checked)
{
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
 * @brief 析构函数，结束程序时
 * @param
 */
MainWindow::~MainWindow()
{
    delete ui;
}




