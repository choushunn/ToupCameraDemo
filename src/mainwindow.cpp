#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "qstylefactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
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
    //系统自带的QStyle风格
    QStringList listStyle = QStyleFactory::keys();
    //打印当前系统支持的系统风格
    foreach(QString val, listStyle){
        qDebug()<<"AppInit: QStyle" << val;
        ui->comboBox_3->addItem(val);
    }

    // 设置相机相关
    getCameraList();

    // 设置WebSocketServer相关
    m_webSocketServer = m_deviceManager.initWebSocketServer();
    // 获得当前设备所有 IP 地址列表
    QList<QHostAddress> ipList = m_webSocketServer->getAllIpAddresses();

    // 将 IP 地址列表添加到 ComboBox 控件中
    for (int i = 0; i < ipList.size(); i++) {
        const QHostAddress &ip = ipList.at(i);
        ui->m_ipComboBox->addItem(ip.toString());
    }

    // 设置相机控件默认值
    ui->expGainSlider->setEnabled(false);
    ui->expTimeSlider->setEnabled(false);
    ui->expTargetlSlider->setEnabled(false);
    ui->m_cmb_resolution->setEnabled(false);

    // 设置ONNX
//    onnx = COnnx::createInstance(model_name, m_isGPU);
}


void MainWindow::setCameraUI(){
    // 设置自动曝光按钮为选中
    ui->autoExpocheckBox->setCheckState(Qt::CheckState::Checked);
    ui->autoExpocheckBox->setEnabled(true);
    // 设置滑动条按钮不可用
    ui->expTimeSlider->setEnabled(false);
    ui->expGainSlider->setEnabled(false);
    std::vector<std::string> res;
    m_camera->getResolution(res);
    ui->m_cmb_resolution->clear();
    // 将分辨率列表添加到combox1中
    for (const auto& str : res) {
        ui->m_cmb_resolution->addItem(QString::fromStdString(str));
    }
    ui->m_cmb_resolution->setEnabled(true);
}

/**
 * @brief findCameraIndex 根据ID查询/dev/video的序号
 * @param cameraId 读取的相机id
 */
int findCameraIndex(QByteArray cameraId){
    std::string dev_name_str(cameraId.data(), cameraId.size());
    std::size_t pos = dev_name_str.find("/dev/video");
    int dev_num =0;
    if (pos != std::string::npos) {
        std::string num_str = dev_name_str.substr(pos + 10);
        dev_num = std::stoi(num_str);
        qDebug() << "Device number: " << dev_num;
        return dev_num;
    }
    return 0;
}

void MainWindow::getCameraList(){
    // 读取相机列表
    ui->m_cbx_camera_list->clear();
    // 创建并初始化camera对象
    m_camera = m_deviceManager.initCamera(m_camType);
    // 获取camera对象
    std::vector <std::string> camera_list;
    m_camera->getCameraList(camera_list);
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
            int cbx_camera_index = ui->m_cbx_camera_list->currentIndex();
            QByteArray cameraId = camera_list[cbx_camera_index];
            m_camIndex = findCameraIndex(cameraId);
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
    m_readTimer = new QTimer();

    // 初始化 JSON 对象
    m_jsonObj["type"] = "text";
    m_jsonObj["content"] = "";
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
    m_appEvent.registerReceiver(this, "sendCameraFrame");
}

void MainWindow::unregisterEvents()
{
    // 取消连接信号和槽，取消注册事件
    // 取消注册MainWindow的clicked事件
    AppEvent::instance().unregisterReceiver(this, "clicked");
}

void MainWindow::signalSlotConnect(){
    // 相机读取图像
    connect(m_readTimer, &QTimer::timeout, this, &MainWindow::showCameraFrame);
    // 显示

    // 将 WebSocket 服务器的 textMessageReceived 信号连接到 onWebSocketMessageReceived 槽函数上
    connect(m_webSocketServer, &CWebSocketServer::textMessageReceived, this, &MainWindow::onWebSocketTextMessageReceived);


    // 将 WebSocket 服务器的 binaryMessageReceived 信号连接到 onWebSocketBinaryMessageReceived 槽函数上
    connect(m_webSocketServer, &CWebSocketServer::binaryMessageReceived, this, &MainWindow::onWebSocketBinaryMessageReceived);

    // 将 WebSocket 服务器的 newConnection 信号连接到 onWebSocketClientConnected 槽函数上
    connect(m_webSocketServer, &CWebSocketServer::clientConnected, this, &MainWindow::onWebSocketClientConnected);

    // 将 WebSocket 服务器的 clientDisconnected 信号连接到 onWebSocketClientDisconnected 槽函数上
    connect(m_webSocketServer, &CWebSocketServer::clientDisconnected, this, &MainWindow::onWebSocketClientDisconnected);

//    //     连接信号和槽
//    QObject::connect(ui->pushButton_4, &QPushButton::clicked, &m_appEvent,
//                     std::bind(&AppEvent::sendEvent,  &m_appEvent, "clicked", std::placeholders::_1));

//    QObject::connect(ui->checkBox_2, &QCheckBox::stateChanged, &m_appEvent,
//                     std::bind(&AppEvent::sendEvent,  &m_appEvent, "check", std::placeholders::_1));
}


// 槽函数
void MainWindow::handleEvent(const QString& eventName, const QVariant& eventData){
    // 处理事件
    qDebug()  << "MainWindow:" << eventName << eventData.toBool();
}

void MainWindow::showCameraFrame()
{
    cv::Mat frame;
    m_camera->read(frame);
    if (frame.empty()) {
       return;
    }
    cv::resize(frame, frame, cv::Size(640, 480));
    cv::Mat output_image;
    if(ui->m_btn_load_algorithm->isChecked()){
        // 算法处理输出图像
        onnx->run(frame, output_image);
    }else{
        output_image = frame.clone();
    }
    // 处理输出图像
    // processor.process(output_image);

    // 显示读取的图像
    if(ui->m_cbx_camera_type->currentText()=="USB"){
        cv::cvtColor(frame,frame,cv::COLOR_BGR2RGB);
        cv::cvtColor(output_image, output_image,cv::COLOR_BGR2RGB);
    }
    QSize size;
    // 没有选择单窗口则显示原始图像
    if(!ui->checkBox_result_window->isChecked()){
        size = ui->m_lbl_display1->size();
        QImage qimage1(frame.data, frame.cols, frame.rows, QImage::Format_RGB888);
        QPixmap pixmap1 = QPixmap::fromImage(qimage1);
        pixmap1 = pixmap1.scaled(size, Qt::KeepAspectRatio);
        ui->m_lbl_display1->setPixmap(pixmap1);
        cv::resize(output_image, output_image, cv::Size(640, 480));
    }else{
        size = ui->m_lbl_display2->size();
    }

//    // 进行锐化处理
//    cv::Mat kernel = (cv::Mat_<float>(3,3) <<  0, -1,  0,
//                  -1,  5, -1,
//                  0, -1,  0);
//    cv::filter2D(frame, output_image, CV_8UC3, kernel);


    // 显示处理后的图像
    QImage qimage(output_image.data, output_image.cols, output_image.rows, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimage);
    pixmap = pixmap.scaled(size, Qt::KeepAspectRatio);
    ui->m_lbl_display2->setPixmap(pixmap);
}


// 信号槽函数
void MainWindow::on_m_btn_open_camera_clicked()
{
    // 打开相机
//    qDebug() << m_camera->isOpened();
    if (m_camera->isOpened()) {
        if(m_readTimer->isActive()){
            m_readTimer->stop();
        };
        m_camera->close();
        ui->m_lbl_display1->clear();
        ui->m_lbl_display2->clear();
        qDebug() << "MainWindow:Camera closed successfully!";
        ui->m_btn_open_camera->setText("打开");
        ui->m_cmb_resolution->setEnabled(false);
    } else {
        // 创建并初始化camera对象
        m_camera = m_deviceManager.initCamera(m_camType);
        // 根据索引打开相机
        bool success = m_camera->open(m_camIndex);
        if (success) {
            m_readTimer->setInterval(int(1000/m_fps));
            m_readTimer->start();
            qDebug() << "MainWindow:Camera opened successfully!";
            ui->m_btn_open_camera->setText("关闭");
            setCameraUI();
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


void MainWindow::on_action_about_triggered() {
    QMessageBox::about(this, "关于", "作者: Spring.");
}



void MainWindow::on_m_btn_open_web_socket_clicked()
{
    if (m_webSocketServer->isListening()) {
        // WebSocket 服务器正在监听，需要关闭服务器
        m_webSocketServer->stop();
        ui->m_btn_open_web_socket->setText(tr("打开"));
        ui->statusbar->showMessage(tr("WebSocket server stopped."));
    } else {
        // WebSocket 服务器未启动，需要启动服务器
        bool success = m_webSocketServer->start(m_port);
        if (success) {
            ui->m_btn_open_web_socket->setText(tr("关闭"));
            ui->statusbar->showMessage(tr("WebSocket server started on port %1.").arg(m_port));
        } else {
            ui->statusbar->showMessage(tr("Failed to start WebSocket server on port %1.").arg(m_port));
        }
    }
}

void MainWindow::on_m_line_port_textChanged(const QString &arg1)
{
    m_port = arg1.toInt();
    qDebug() << "Mainwindow:m_port is " << m_port;
}

void MainWindow::onWebSocketTextMessageReceived(QWebSocket *clientSocket, const QString& message)
{
    ui->textBrowser->append(QString("[%1]: %2").arg(clientSocket->peerAddress().toString()).arg(message));
}

void MainWindow::onWebSocketBinaryMessageReceived(QWebSocket *clientSocket, const QByteArray& message)
{
    QString str = QString::fromUtf8(message);
    ui->textBrowser->append(QString("[%1]: %2").arg(clientSocket->peerAddress().toString()).arg(str));
}


void MainWindow::onWebSocketClientConnected(QWebSocket *clientSocket)
{
    // 获取客户端的 IP 和端口号
    QString ip = clientSocket->peerAddress().toString();
    int port = clientSocket->peerPort();

    // 将客户端的 IP 和端口号添加到 comboBox 控件中
    ui->comboBox_4->addItem(QString("%1:%2").arg(ip).arg(port));

}

void MainWindow::onWebSocketClientDisconnected(QWebSocket *clientSocket)
{
    // 获取客户端的 IP 和端口号
    QString ip = clientSocket->peerAddress().toString();
    int port = clientSocket->peerPort();

    // 从 comboBox 控件中移除客户端的 IP 和端口号
    ui->comboBox_4->removeItem(ui->comboBox_4->findText(QString("%1:%2").arg(ip).arg(port)));
}



void MainWindow::on_comboBox_3_currentTextChanged(const QString &arg1)
{
    //设置当前风格为
    qApp->setStyle(QStyleFactory::create(arg1));
}


void MainWindow::on_pushButton_2_clicked()
{
    // 保存图像
    m_camera->getSnap();
}


void MainWindow::on_checkBox_4_stateChanged(int arg1)
{

    if(arg1){
        m_isGPU = true;
        qDebug() << "MainWindow:当前ONNX设备GPU";
    }else{
        qDebug() << "MainWindow:当前ONNX设备CPU";
        m_isGPU=false;
    }
}


void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    model_name = arg1.toStdString();
    // 切换算法才创建
    onnx = COnnx::createInstance(model_name, m_isGPU);
}




void MainWindow::on_m_btn_load_algorithm_clicked(bool checked)
{
    if(checked){
        if(onnx != nullptr){
            onnx = COnnx::createInstance(model_name, m_isGPU);
        }
    }
}


void MainWindow::on_m_btn_send_message_clicked()
{
    // 获取要发送的消息
    QString message = ui->plainTextEdit->toPlainText();
    if (message.isEmpty()){
        QMessageBox::information(this, "提示", "发送的信息为空");
        return;
    }

    // 获取当前选中的客户端
    QString clientStr = ui->comboBox_4->currentText();
    if (clientStr.isEmpty()){
        QMessageBox::information(this, "提示", "未选择客户端");
        return;
    }

    // 查找与客户端对应的 WebSocket 连接
    QList<QWebSocket*> clientSockets = m_webSocketServer->findClientSockets(clientStr);
    if (clientSockets.isEmpty()){
        qDebug() << "MainWindow:clientSockets is null";
        return;
    }

    // 封装消息到 JSON 对象中
    m_jsonObj["type"] = "image";
    m_jsonObj["data1"] = 1;
    m_jsonObj["data2"] = "Spring";
    //    m_jsonObj["data"] = QString::fromLatin1(imageData.toBase64().data());

    // 将 JSON 对象转换为 JSON 文档，并将其发送到客户端
    QJsonDocument docObj;
    docObj.setObject(m_jsonObj);

    // 遍历所有与客户端对应的 WebSocket 连接，发送消息
    for (QWebSocket* client : clientSockets) {
        m_webSocketServer->sendBinaryMessage(client, docObj.toJson());
    }
}


void MainWindow::on_horizontalSlider_8_valueChanged(int value)
{
    // 帧率改变
    m_fps= value;
    // 定时器改变
    m_readTimer->setInterval(int(1000/m_fps));
    m_readTimer->start();
    ui->label_fps->setText(QString::number(value));
}


void MainWindow::on_expTargetlSlider_valueChanged(int value)
{
    m_camera->setExpoTarget(value);
    ui->label_exp_target->setText(QString::number(value));
}


void MainWindow::on_expTimeSlider_valueChanged(int value)
{
    m_camera->setExpoTime(value);
    double valueInMs = value / 1000.0;
    QString valueString = QString::number(valueInMs, 'f', 3) + "ms";
    ui->label_exp_time->setText(valueString);
}


void MainWindow::on_expGainSlider_valueChanged(int value)
{
    m_camera->setExpoGain(value);
    QString valueString = QString::number(value) + "%";
    ui->label_exp_gain->setText(valueString);
}


void MainWindow::on_autoExpocheckBox_stateChanged(int arg1)
{
    if(m_camType == "TOUP"){
        //设置相机自动曝光
        m_camera->setAutoExpo(arg1);
        qDebug() << "setAutoExpo:" << arg1;
        ui->expTimeSlider->setEnabled(!arg1);
        ui->expGainSlider->setEnabled(!arg1);
        // 设置曝光时间初始值
        Context ctx;
        m_camera->getContext(ctx);
        // 曝光时间
        ui->expTimeSlider->setRange(ctx.uimin, ctx.uimax);
        ui->expTimeSlider->setValue(ctx.uidef);
        // 增益
        ui->expGainSlider->setRange(ctx.usmin, ctx.usmax);
        ui->expGainSlider->setValue(ctx.usdef);
    }else{
        ui->expTimeSlider->setEnabled(false);
        ui->expGainSlider->setEnabled(false);
    }
}

void MainWindow::on_btn_open_dialog_clicked()
{

}


void MainWindow::on_m_cmb_resolution_currentIndexChanged(int index)
{
    //分辨率切换
    m_camera->setResolution(index);
}


void MainWindow::on_checkBox_result_window_stateChanged(int arg1)
{
    // 选中则只显示结果窗口
    if(arg1){
        ui->m_lbl_display1->setVisible(false);
    }else{
        ui->m_lbl_display1->setVisible(true);
    }
}

