#include "mainwindow.h"
#include "./ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_readTimer = new QTimer();
    setupUI();
    setDefaultValues();
    setDefaultStates();
    registerEvents();
    signalSlotConnect();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUI()
{
    // 设置控件的默认值和参数
    getCameraList();
}

void MainWindow::getCameraList(){
    // 读取相机列表
    ui->m_cbx_camera_list->clear();
    m_deviceManager.initCamera(m_camType, m_camIndex);
    std::vector <std::string> camera_list;
    m_deviceManager.getCameraList(camera_list);
    if(camera_list.empty()){
        ui->m_cbx_camera_list->setEditable(true);
        ui->m_cbx_camera_list->setCurrentText("未检测到相机");
        ui->m_cbx_camera_list->setDisabled(true);
        ui->m_btn_open_camera->setDisabled(true);
        qDebug() << "MainWindow:相机初始化失败." << "检测到" << 0 << "个相机";
        return;
    }else{
        ui->m_cbx_camera_list->setEditable(false);
        ui->m_cbx_camera_list->setDisabled(false);
        ui->m_btn_open_camera->setDisabled(false);
        for (const std::string &camera: camera_list) {
            ui->m_cbx_camera_list->addItem(camera.c_str());
        }
        if(m_camType == "USB"){
#ifdef _WIN32
            m_camIndex = ui->m_cbx_camera_list->currentIndex();
#else
            QByteArray dev_name = m_cameraList[m_cameraIndex].id();
            m_cameraIndex = findCameraIndex(dev_name);
#endif
        }else{

            m_camIndex = ui->m_cbx_camera_list->currentIndex();
        }
        qDebug() << "MainWindow:相机初始化完成." << "检测到"<< camera_list.capacity() << "个相机.";
    }
}

void MainWindow::setDefaultValues()
{
    // 设置默认值
}

void MainWindow::setDefaultStates()
{
    // 设置默认状态
}
void MainWindow::registerEvents()
{
    // 连接信号和槽，注册事件
    // 注册MainWindow的clicked事件
    m_appEvent.registerReceiver(this, "clicked");
    m_appEvent.registerReceiver(this, "check");
    m_appEvent.registerReceiver(&m_deviceManager, "sendCameraFrame");
}

void MainWindow::unregisterEvents()
{
    // 取消连接信号和槽，取消注册事件
    // 取消注册MainWindow的clicked事件
    AppEvent::instance().unregisterReceiver(this, "clicked");
}

void MainWindow::signalSlotConnect(){
    // 读取
    connect(m_readTimer, &QTimer::timeout, &m_deviceManager, &DeviceManager::readCamera);
    // 显示
    connect(&m_deviceManager, &DeviceManager::sendCameraFrame, this, &MainWindow::showCameraFrame);
    //     连接信号和槽
    QObject::connect(ui->pushButton_4, &QPushButton::clicked, &m_appEvent,
                     std::bind(&AppEvent::sendEvent,  &m_appEvent, "clicked", std::placeholders::_1));

    QObject::connect(ui->checkBox_2, &QCheckBox::stateChanged, &m_appEvent,
                     std::bind(&AppEvent::sendEvent,  &m_appEvent, "check", std::placeholders::_1));
}


// 槽函数
void MainWindow::handleEvent(const QString& eventName, const QVariant& eventData){
    // 处理事件
    qDebug()  << "MainWindow:" << eventName << eventData.toBool();
}

void MainWindow::showCameraFrame(cv::Mat& frame)
{
    // 显示读取的图像
    cv::resize(frame, frame, cv::Size(640, 480));
    QSize size = ui->m_lbl_display1->size();
    QImage qimage1(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
    QPixmap pixmap1 = QPixmap::fromImage(qimage1);
    pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display1->setPixmap(pixmap1);

    // 显示处理后的图像
    cv::Mat output_image = frame.clone();

    // 处理输出图像
    // processor.process(output_image);

    cv::resize(output_image, output_image, cv::Size(640, 480));
    QImage qimage(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimage);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display2->setPixmap(pixmap);
}


// 信号槽函数
void MainWindow::on_m_btn_open_camera_clicked()
{
    // 打开相机
    if (m_deviceManager.cameraOpened()) {
        if(m_readTimer->isActive()){
            m_readTimer->stop();
        };
        m_deviceManager.closeCamera();
        ui->m_lbl_display1->clear();
        ui->m_lbl_display2->clear();

//        QMessageBox::information(this, "Camera closed", "Camera closed successfully!");
        ui->m_btn_open_camera->setText("Open Camera");
    } else {
        m_deviceManager.initCamera(m_camType, m_camIndex);
        bool success = m_deviceManager.openCamera(m_camIndex);
        if (success) {
            m_readTimer->setInterval(int(1000/m_fps));
            m_readTimer->start();
//            QMessageBox::information(this, "Camera opened", "Camera opened successfully!");
            ui->m_btn_open_camera->setText("Close Camera");
        } else {
            QMessageBox::critical(this, "Error", "Failed to open camera!");
        }
    }
}


void MainWindow::on_m_cbx_camera_list_currentIndexChanged(int index)
{
    // 相机索引改变
    m_camIndex = index;
}


void MainWindow::on_m_cbx_camera_type_currentIndexChanged(int index)
{
//    m_camType = ui->m_cbx_camera_type->currentText().toStdString();
//    qDebug()<<"MainWindow:m_cbx_camera_type_currentIndexChanged" << index;
}


void MainWindow::on_m_cbx_camera_type_currentTextChanged(const QString &arg1)
{
    // 相机类型改变
    m_camType = arg1.toStdString();
    qDebug()<<"MainWindow:m_cbx_camera_type_currentTextChanged" << arg1;
    // 更新相机列表
    getCameraList();

}

void MainWindow::on_m_btn_reset_clicked()
{
    // 关闭相机
//    m_deviceManager.cameraClosed();
    // 重新初始化相机
//    getCameraList();
}


void MainWindow::on_action_open_image_triggered()
{
    QString path = QDir::currentPath();
    QString fileName = QFileDialog::getOpenFileName(this, "选择一个文件", path,  "图像文件 (*.jpg *.png *.bmp)");

    // 如果选择了文件，则加载图像文件并显示在标签控件中
    if (!fileName.isEmpty())
    {
        QPixmap pixmap(fileName);
        ui->m_lbl_display1->setPixmap(pixmap);
        ui->m_lbl_display2->setPixmap(pixmap);
    }
}


void MainWindow::on_action_fullscreen_triggered()
{
    //设置全屏
    if(MainWindow::isFullScreen()){
        MainWindow::showNormal();
    }else{
        MainWindow::showFullScreen();
    }
}

void MainWindow::on_action_maxscreen_triggered()
{
    //最大化窗口
    if(MainWindow::isMaximized()){
        MainWindow::showNormal();
    }else{
        MainWindow::showMaximized();
    }
}

void MainWindow::on_action_normal_triggered()
{
    MainWindow::showNormal();
}

void MainWindow::on_action_exit_triggered()
{
    //退出程序
    int ret = QMessageBox::warning(this, "退出", "是否退出程序", QMessageBox::Ok, QMessageBox::Cancel);
    switch(ret)
    {
    case QMessageBox::Ok:
        qDebug() <<"退出程序";
        QApplication::quit();
        break;
    case QMessageBox::Cancel:
        qDebug() <<"取消退出程序";
        break;
    default:
        break;
    }
}



//void MainWindow::on_pushButton_clicked() {
//    //    创建一个窗口对象
//    Dialog dialog(this);
//    //    调用显示窗口的函数
//    dialog.exec();
//}


void MainWindow::on_action_about_triggered() {
    QMessageBox::about(this, "关于", "作者: Spring.");
}



